#include "TcpConnection.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <strings.h>
#include <iostream>
#include "Channel.h"

TcpConnection::TcpConnection(EventLoop* loop, int sockfd): loop_(loop),
                                                       cfd_(sockfd), pChannel_(nullptr)
{
    pChannel_ = new Channel(loop_, cfd_);
    pChannel_->setCallBack(this);
    pChannel_->enableRead();
    std::cout << "new connection, socket: " << sockfd << std::endl;
}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::OnIn(int sockfd)
{
    char buf[1024];
    bzero(buf, sizeof buf);

    int ret = read(sockfd, buf, 1024);
    if(ret < 0)
    {
        perror("read error");
        if(errno == ECONNRESET) close(sockfd);
        std::cout << "connection close, socket: " << sockfd << std::endl;
    }
    else if(ret == 0)
    {
        close(sockfd);
        std::cout << "connection close, socket: " << sockfd << std::endl;
    }
    else
    {
        int writelen = write(sockfd, buf, ret);
        if(writelen != ret)    perror("write error");
    }

}

