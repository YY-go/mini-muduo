#include "Channel.h"
#include <sys/epoll.h>
#include <stdio.h>
#include "IChannelCallBack.h"
#include "EventLoop.h"


Channel::Channel(EventLoop* loop, int sockfd) : sockfd_(sockfd), event_(0), 
                                            revent_(0), loop_(loop), callBack_(nullptr)
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
    if(callBack_ && revent_ & EPOLLIN) callBack_->OnIn(sockfd_);
}

void Channel::setRevent(int revent)
{
    revent_ = revent;
}

void Channel::enableRead()
{
    event_ |= EPOLLIN;
    update();
}

int Channel::getEvents()
{
    return event_;
}

int Channel::getSockfd()
{
    return sockfd_;
}

void Channel::update()
{
    loop_->update(this);
}

