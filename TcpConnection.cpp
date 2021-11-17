#include "TcpConnection.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <strings.h>
#include <iostream>
#include "Channel.h"
#include "IMuduoUser.h"
#include "EventLoop.h"
#include "Task.h"

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
            Task task(this);
            loop_->queueInLoop(task);
        }
    }
}

void TcpConnection::sendInLoop(const std::string& message)
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
            {
                Task task(this);
                loop_->queueInLoop(task);
            }

        }

    }
}

void TcpConnection::send(const std::string& message)
{
    if(loop_->isInLoopThread())
        sendInLoop(message);
    else
    {
        Task task(this, message, this);
        loop_->runInLoop(task);
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

int TcpConnection::getEventLoopTid()
{
    return loop_->getTid();
}

void TcpConnection::run0()
{
    pUser_->OnWriteComplete(this);
}

void TcpConnection::run2(const std::string& message, void* param)
{
    sendInLoop(message);
}

