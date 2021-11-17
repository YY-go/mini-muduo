#include "Channel.h"
#include <sys/epoll.h>
#include <stdio.h>
#include "IChannelCallBack.h"


Channel::Channel(int epollfd, int sockfd) : epollfd_(epollfd), sockfd_(sockfd),
                                            event_(0), revent_(0), callBack_(nullptr)
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

void Channel::update()
{
    struct epoll_event ev;
    ev.data.ptr = this;
    ev.events = event_;
    int ret = epoll_ctl(epollfd_, EPOLL_CTL_ADD, sockfd_, &ev);
    if(ret == -1)   perror("epoll_ctl error");
}

