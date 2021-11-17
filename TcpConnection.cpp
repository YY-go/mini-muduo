#include "TcpConnection.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <strings.h>
#include <iostream>
#include "Channel.h"
#include "IMuduoUser.h"

TcpConnection::TcpConnection(EventLoop* loop, int sockfd): loop_(loop), cfd_(sockfd), 
                                                           pChannel_(nullptr), pUser_(nullptr)
{
    pChannel_ = new Channel(loop_, cfd_);
    pChannel_->setCallBack(this);
    pChannel_->enableRead();
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
        std::string mes(buf, ret);
        pUser_->OnMessage(this, mes);
    }

}

void TcpConnection::send(const std::string& message)
{
    int n = ::write(cfd_, message.c_str(), message.size());
    if(n != message.size()) 
    {
        perror("write error: ");
        std::cout << message.size() - n << "Bytes left" << std::endl;
    }
}

void TcpConnection::setUser(IMuduoUser* pUser)
{
    pUser_ = pUser;
}

void TcpConnection::connectEstablished()
{
    if(pUser_) pUser_->OnConnection(this);
}

int TcpConnection::getSocket()
{
    return cfd_;
}

