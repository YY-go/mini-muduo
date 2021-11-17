#include "Epoll.h"
#include "stdio.h"
#include "Channel.h"

Epoll::Epoll()
{
    epollfd_ = epoll_create1(EPOLL_CLOEXEC);
    if(epollfd_ == -1)   perror("epoll_create1 error");
}

Epoll::~Epoll()
{

}

void Epoll::poll(std::vector<Channel*> & pChannels)
{
    int fds = epoll_wait(epollfd_, events_, max_events, -1);
    if(fds == -1)
    {
        perror("epoll_wait error");
        return ;
    }

    for(int i = 0; i < fds; ++i)
    {
        Channel* pChannel =  static_cast<Channel*>(events_[i].data.ptr);
        pChannel->setRevent(events_[i].events);
        pChannels.push_back(pChannel);
    }

}

void Epoll::update(Channel* pChannel)
{
    struct epoll_event ev;
    ev.data.ptr = pChannel;
    ev.events = pChannel->getEvents();
    int fd = pChannel->getSockfd();
    int ret = epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev);
    if(ret == -1)   perror("epoll_ctl error");
}
  
