#include "TimerQueue.h"
#include "Channel.h"
#include <unistd.h>
#include <sys/timerfd.h>
#include <string.h>
#include "EventLoop.h"

TimerQueue::TimerQueue(EventLoop* loop)
    : timerfd_(createTimerfd())
    , loop_(loop)
    , timerfdChannel_(new Channel(loop_, timerfd_))
    , pAddTimerWrapper_(new AddTimerWrapper(this))
    , pCancelTimerWrapper_(new CancelTimerWrapper(this))
{
    timerfdChannel_->setCallBack(this);
    timerfdChannel_->enableRead();
}

TimerQueue::~TimerQueue()
{
    ::close(timerfd_);
}

void TimerQueue::doAddTimer(void* param)
{
    Timer* pTimer = static_cast<Timer*>(param);
    bool earliestChanged = insert(pTimer);
    if(earliestChanged)
        resetTimerfd(timerfd_, pTimer->getStamp());
}

void TimerQueue::doCancelTimer( void* param )
{
    Timer* pTimer = static_cast<Timer*>(param);
    for(auto e : timers_)
        if(e.second == pTimer)
        {
            timers_.erase(e);
            break;
        }
}

int64_t TimerQueue::addTimer(IRun* pRun, MyTimeStamp when, double interval)
{
    Timer* pTimer = new Timer(when, pRun, interval);
    loop_->queueLoop(pAddTimerWrapper_, pTimer);
    return (int64_t)pTimer;
}

void TimerQueue::cancelTimer(int64_t timerfd)
{
    loop_->queueLoop(pCancelTimerWrapper_, (void*)timerfd);
}

void TimerQueue::handleRead()
{
    MyTimeStamp now(MyTimeStamp::now());
    readTimerfd(timerfd_, now);

    std::vector<Entry> expired = getExpired(now);
    for(auto entry : expired)
        entry.second->run();
    reset(expired, now);
}

void TimerQueue::handleWrite()
{

}

int TimerQueue::createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                   TFD_CLOEXEC | TFD_NONBLOCK);
    if(timerfd == -1) perror("timerfd_create error");
    return timerfd;
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(MyTimeStamp now)
{
    std::vector<TimerQueue::Entry> expired;
    Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    TimerList::iterator end = timers_.lower_bound(sentry);
    copy(timers_.begin(), end, back_inserter(expired));
    timers_.erase(timers_.begin(), end);
    return expired;
}

void TimerQueue::readTimerfd(int timerfd, MyTimeStamp now)
{
    uint64_t howmany;
    ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
    if(n == -1) perror("readTimerfd error");
}

void TimerQueue::reset(const std::vector<Entry>& expired, MyTimeStamp now)
{
    for(auto entry : expired)
    {
        if (entry.second->isRepeat())
        {
            entry.second->moveToNext();
            insert(entry.second);
        }
    }

    MyTimeStamp nextExpire;
    if(!timers_.empty())
    {
        nextExpire = timers_.begin()->second->getStamp();
    }
    if(nextExpire.valid())
    {
        resetTimerfd(timerfd_, nextExpire);
    }
}

void TimerQueue::resetTimerfd(int timerfd, MyTimeStamp stamp)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof newValue);
    bzero(&oldValue, sizeof oldValue);
    newValue.it_value = howMuchTimeFromNow(stamp);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if(ret == -1) perror("timerfd_settime error");
}

bool TimerQueue::insert(Timer* pItem)
{
    bool earliestChanged = false;
    MyTimeStamp when = pItem->getStamp();
    auto it = timers_.begin();
    if(it == timers_.end() || when < it->first)
        earliestChanged = true;

    std::pair<TimerList::iterator, bool> result
        = timers_.insert(Entry(when, pItem));
    if(!result.second) perror("timers_.insert() error");

    return earliestChanged;
}

struct timespec TimerQueue::howMuchTimeFromNow(MyTimeStamp when)
{
    int64_t microseconds = when.microSecondsSinceEpoch()
        - MyTimeStamp::now().microSecondsSinceEpoch();
    if(microseconds < 100)
        microseconds = 100;
    struct timespec ts;
    ts.tv_sec = microseconds / MyTimeStamp::kMicroSecondsPerSecond;
    ts.tv_nsec = (microseconds % MyTimeStamp::kMicroSecondsPerSecond) * 1000;
    return ts;
}

