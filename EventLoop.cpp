#include "IRun.h"
#include "EventLoop.h"
#include <unistd.h>
#include "Epoll.h"
#include "Channel.h"
#include <sys/eventfd.h>
#include <stdio.h>

EventLoop::EventLoop() : quit_(false), poller_(new Epoll())
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

void EventLoop::queueLoop(IRun* pRun)
{
    PendingFunctors_.push_back(pRun);
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
    std::vector<IRun*> tempRuns;
    tempRuns.swap(PendingFunctors_);
    for(auto f : tempRuns) f->run();
}

int EventLoop::createEventfd()
{
    int n = eventfd(0, 0);
    if(n == -1) perror("eventfd error");
    return n;
}

