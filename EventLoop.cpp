#include "IRun.h"
#include "EventLoop.h"
#include <unistd.h>
#include "Epoll.h"
#include "Channel.h"
#include <sys/eventfd.h>
#include <stdio.h>
#include "TimerQueue.h"
#include "CurrentThread.h"

EventLoop::EventLoop() 
    : quit_(false)
    , callingPendingFunctors_(false)
    , poller_(new Epoll())
    , threadId_(CurrentThread::tid())
    , pTimerQueue_(new TimerQueue(this))
{
    eventfd_ = createEventfd();
    pWakeupChannel_ = new Channel(this, eventfd_);
    pWakeupChannel_->setCallBack(this);
    pWakeupChannel_->enableRead();
}

EventLoop::~EventLoop()
{

}

void EventLoop::loop()
{
    while(!quit_)
    {
        std::vector<Channel*> channels;
        poller_->poll(channels);

        for(auto ch : channels)
            ch->handleEvent();

        doPendingFunctors();
    }
}

void EventLoop::update(Channel* pChannel)
{
    poller_->update(pChannel);
}

void EventLoop::queueInLoop(Task& task)
{
    {
        MutexLockGuard guard(mutex_);
        pendingFunctors_.push_back(task);
    }

    if(!isInLoopThread() || callingPendingFunctors_)
        wakeup();
}

void EventLoop::runInLoop(Task& task)
{
    if(isInLoopThread())
        task.doTask();
    else
        queueInLoop(task);
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(eventfd_, &one, sizeof one);
    if(n != sizeof one) perror("wakeup error");
}

void EventLoop::handleRead()
{
    uint64_t one;
    ssize_t n = ::read(eventfd_, &one, sizeof one);
    if(n != sizeof one) perror("read error");
}


void EventLoop::handleWrite()
{

}

void EventLoop::doPendingFunctors()
{
    std::vector<Task> tempRuns;
    callingPendingFunctors_ = true;
    {
        MutexLockGuard guard(mutex_);
        tempRuns.swap(pendingFunctors_);
    }
    for(auto f : tempRuns) f.doTask();
    callingPendingFunctors_ = false;
}

int EventLoop::createEventfd()
{
    int n = eventfd(0, 0);
    if(n == -1) perror("eventfd error");
    return n;
}

int64_t EventLoop::runAt(MyTimeStamp when, IRun0* pRun)
{
    return pTimerQueue_->addTimer(pRun, when, 0.0);
}

int64_t EventLoop::runAfter(double delay, IRun0* pRun)
{
    return pTimerQueue_->addTimer(pRun, MyTimeStamp::nowAfter(delay), 0.0);
}

int64_t EventLoop::runEvery(double interval, IRun0* pRun)
{
    return pTimerQueue_->addTimer(pRun, MyTimeStamp::nowAfter(interval), interval);
}

void EventLoop::cancelTimer(int64_t timerId)
{
    pTimerQueue_->cancelTimer(timerId);
}

bool EventLoop::isInLoopThread()
{
    return threadId_ == CurrentThread::tid();
}

pid_t EventLoop::getTid()
{
    return threadId_;
}

