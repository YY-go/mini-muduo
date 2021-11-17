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

using namespace std;

TcpServer::TcpServer() : epollfd_(-1), lfd_(-1)
{

}

TcpServer::~TcpServer()
{

}

int TcpServer::init()
{
    int lfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if(lfd == -1)   perror("socket error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int on = 1;
    setsockopt(lfd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof on);
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof on);

    int ret;
    ret = bind(lfd, (struct sockaddr*) &serv_addr, sizeof serv_addr);
    if(ret == -1)   perror("bind error");

    ret = listen(lfd, 1024);
    if(ret == -1)   perror("listen error");

    return lfd;
}

void TcpServer::OnIn(int sockfd)
{
    
    if(sockfd == lfd_)
    {
        struct sockaddr_in clet_addr;
        bzero(&clet_addr, sizeof clet_addr);
        socklen_t clet_addr_len = sizeof clet_addr;

        int cfd = accept4(lfd_, (struct sockaddr*) &clet_addr, &clet_addr_len, SOCK_CLOEXEC | SOCK_NONBLOCK);
        if(cfd == -1)   perror("accept error");

        Channel* pChannel = new Channel(epollfd_, cfd);
        pChannel->setCallBack(this);
        pChannel->enableRead();

        cout << "new connection, socket: " << cfd << endl;
    }
    else
    {
        char buf[1024];
        bzero(buf, sizeof buf);

        int ret = read(sockfd, buf, 1024);
        if(ret < 0)
        {
            perror("read error");
            if(errno == ECONNRESET) close(sockfd);
            cout << "connection close, socket: " << sockfd << endl;
        }
        else if(ret == 0)
        {
            close(sockfd);
            cout << "connection close, socket: " << sockfd << endl;
        }
        else
        {
            int writelen = write(sockfd, buf, ret);
            if(writelen != ret)    perror("write error");
        }
    }
}

void TcpServer::start()
{

    epollfd_ = epoll_create1(EPOLL_CLOEXEC);
    if(epollfd_ == -1)   perror("epoll_create1 error");

    lfd_ = init();
    Channel* pChannel = new Channel(epollfd_, lfd_);
    pChannel->setCallBack(this);
    pChannel->enableRead();
    
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
