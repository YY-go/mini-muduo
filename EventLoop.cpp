#include "EventLoop.h"
#include "Epoll.h"
#include <vector>
#include "Channel.h"

EventLoop::EventLoop() : quit_(false), poller_(new Epoll())
{

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
    }
}

void EventLoop::update(Channel* pChannel)
{
    poller_->update(pChannel);
}

