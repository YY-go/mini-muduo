#include "TcpConnection.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <strings.h>
#include <iostream>
#include "Channel.h"
#include "IMuduoUser.h"

TcpConnection::TcpConnection(EventLoop* loop, int sockfd)
    : loop_(loop)
    , cfd_(sockfd)
    , pChannel_(nullptr)
    , pUser_(nullptr)
    , inBuf_(new std::string)
    , outBuf_(new std::string)
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
        inBuf_->append(buf, readBytes);
        pUser_->OnMessage(this, inBuf_);
    }

}

void TcpConnection::handleWrite()
{
    int writeBytes = ::write(cfd_, outBuf_->c_str(), outBuf_->size());
    if(writeBytes == -1) perror("write error");
    else
    {
        *outBuf_ = outBuf_->substr(writeBytes, outBuf_->size());
        if(outBuf_->empty()) pChannel_->disableWrite();
    }
}

void TcpConnection::send(const std::string& message)
{
    if(pChannel_->isWriting())
    {
        outBuf_->append(message, message.size());
    }
    else
    {
        int writeBytes = ::write(cfd_, message.c_str(), message.size());
        if(writeBytes == -1) perror("wirte error");
        else
        {
            if(writeBytes != message.size()) 
            {
                outBuf_->append(message[writeBytes], message.size() - writeBytes);
                pChannel_->enableWrite();
            }

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

