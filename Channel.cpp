#include "Channel.h"
#include <sys/epoll.h>
#include <stdio.h>
#include "IChannelCallBack.h"
#include "EventLoop.h"


Channel::Channel(EventLoop* loop, int sockfd) 
    : sockfd_(sockfd)
    , event_(0)
    , revent_(0)
    , index_(-1)
    , loop_(loop)
    , callBack_(nullptr)
{

}

Channel::~Channel()
{

}

void Channel::setCallBack(IChannelCallBack* callBack)
{
    callBack_ = callBack;
}

void Channel::handleEvent()
{
    if(callBack_)
    {
        if(revent_ & EPOLLIN) callBack_->handleRead();
        if(revent_ & EPOLLOUT) callBack_->handleWrite();
    }
}

void Channel::setRevent(int revent)
{
    revent_ = revent;
}

void Channel::setIndex(int index)
{
    index_ = index;
}

void Channel::enableRead()
{
    event_ |= EPOLLIN;
    update();
}

void Channel::enableWrite()
{
    event_ |= EPOLLOUT;
    update();
}

void Channel::disableWrite()
{
    event_ &= ~EPOLLOUT;
    update();
}

bool Channel::isWriting()
{
    return event_ & EPOLLOUT;
}

int Channel::getEvents()
{
    return event_;
}

int Channel::getSockfd()
{
    return sockfd_;
}

int Channel::getIndex()
{
    return index_;
}

void Channel::update()
{
    loop_->update(this);
}

