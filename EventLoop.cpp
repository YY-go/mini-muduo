#include "IRun.h"
#include "EventLoop.h"
#include <unistd.h>
#include "Epoll.h"
#include "Channel.h"
#include <sys/eventfd.h>
#include <stdio.h>
#include "TimerQueue.h"

EventLoop::EventLoop() 
    : quit_(false)
    , poller_(new Epoll())
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

void EventLoop::queueLoop(IRun* pRun, void* param)
{
    Runner r(pRun, param);
    PendingFunctors_.push_back(r);
    wakeup();
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
    std::vector<Runner> tempRuns;
    tempRuns.swap(PendingFunctors_);
    for(auto f : tempRuns) f.doRun();
}

int EventLoop::createEventfd()
{
    int n = eventfd(0, 0);
    if(n == -1) perror("eventfd error");
    return n;
}

int64_t EventLoop::runAt(MyTimeStamp when, IRun* pRun)
{
    return pTimerQueue_->addTimer(pRun, when, 0.0);
}

int64_t EventLoop::runAfter(double delay, IRun* pRun)
{
    return pTimerQueue_->addTimer(pRun, MyTimeStamp::nowAfter(delay), 0.0);
}

int64_t EventLoop::runEvery(double interval, IRun* pRun)
{
    return pTimerQueue_->addTimer(pRun, MyTimeStamp::nowAfter(interval), interval);
}

void EventLoop::cancelTimer(int64_t timerfd)
{
    pTimerQueue_->cancelTimer(timerfd);
}

