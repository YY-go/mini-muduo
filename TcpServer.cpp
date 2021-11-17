#include "TcpServer.h"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include "Channel.h"
#include <vector>
#include "Acceptor.h"
#include "TcpConnection.h"

using namespace std;

TcpServer::TcpServer() : epollfd_(-1), pAcceptor_(nullptr)
{

}

TcpServer::~TcpServer()
{

}

void TcpServer::start()
{

    epollfd_ = epoll_create1(EPOLL_CLOEXEC);
    if(epollfd_ == -1)   perror("epoll_create1 error");

    pAcceptor_ = new Acceptor(epollfd_);
    pAcceptor_->setCallBack(this);
    pAcceptor_->start();
    
    while(1)
    {
        std::vector<Channel*> channels;
        int fds = epoll_wait(epollfd_, events_, max_events, -1);
        if(fds == -1)
        {
            perror("epoll_wait error");
            break;
        }

        for(int i = 0; i < fds; ++i)
        {
            Channel* pChannel =  static_cast<Channel*>(events_[i].data.ptr);
            pChannel->setRevent(events_[i].events);
            channels.push_back(pChannel);
        }

        for(auto ch : channels)
        {
            ch->handleEvent();
        }
    }
}

void TcpServer::newConnection(int sockfd)
{
    TcpConnection* pTcpConnecton = new TcpConnection(epollfd_, sockfd);
    connectons_[sockfd] = pTcpConnecton;
}

