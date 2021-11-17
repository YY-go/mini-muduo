#include "TcpConnection.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <strings.h>
#include <iostream>
#include "Channel.h"
#include "IMuduoUser.h"
#include "EventLoop.h"

TcpConnection::TcpConnection(EventLoop* loop, int sockfd)
    : loop_(loop)
    , cfd_(sockfd)
    , pUser_(nullptr)
{
    pChannel_ = new Channel(loop_, cfd_);
    pChannel_->setCallBack(this);
    pChannel_->enableRead();
}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::handleRead()
{
    char buf[1024];
    bzero(buf, sizeof buf);

    int readBytes = read(cfd_, buf, 1024);
    if(readBytes < 0)
    {
        perror("read error");
        if(errno == ECONNRESET) close(cfd_);
        std::cout << "connection close, socket: " << cfd_ << std::endl;
    }
    else if(readBytes == 0)
    {
        close(cfd_);
        std::cout << "connection close, socket: " << cfd_ << std::endl;
    }
    else
    {
        std::string line(buf, readBytes);
        inBuf_.append(line);
        pUser_->OnMessage(this, &inBuf_);
    }

}

void TcpConnection::handleWrite()
{
    int writeBytes = ::write(cfd_, outBuf_.peek(), outBuf_.readableBytes());
    if(writeBytes == -1) perror("write error");
    else
    {
        outBuf_.retrieve(writeBytes); 
        if(outBuf_.readableBytes() == 0)
        {
            pChannel_->disableWrite();
            loop_->queueLoop(this);
        }
    }
}

void TcpConnection::send(const std::string& message)
{
    if(pChannel_->isWriting())
    {
        outBuf_.append(message);
    }
    else
    {
        int writeBytes = ::write(cfd_, message.c_str(), message.size());
        if(writeBytes == -1) perror("wirte error");
        else
        {
            if(writeBytes != message.size()) 
            {
                outBuf_.append(message.substr(writeBytes, message.size()));
                pChannel_->enableWrite();
            }
            else
                loop_->queueLoop(this);

        }

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

void TcpConnection::run()
{
    pUser_->OnWriteComplete(this);
}
