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
#include "Socket.h"
#include "IServerCallBack.h"

TcpConnection::TcpConnection(EventLoop* loop, int sockfd)
    : loop_(loop)
    , cfd_(new Socket(sockfd))
    , pChannel_(new Channel(loop_, cfd_->fd()))
    , pUser_(nullptr)
    , pServerCallBack_(nullptr)
    , highWaterMark_(64*1024*1024)
    , status_(kConnecting)
{
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

    int readBytes = read(cfd_->fd(), buf, 1024);
    if(readBytes < 0)
    {
        perror("read error");
        if(errno == ECONNRESET) handleClose();
    }
    else if(readBytes == 0)
    {
        handleClose();
    }
    else
    {
        std::string line(buf, readBytes);
        inBuf_.append(line);
        pUser_->OnMessage(shared_from_this(), &inBuf_);
    }

}

void TcpConnection::handleWrite()
{
    int writeBytes = ::write(cfd_->fd(), outBuf_.peek(), outBuf_.readableBytes());
    if(writeBytes == -1) perror("write error");
    else
    {
        outBuf_.retrieve(writeBytes); 
        if(outBuf_.readableBytes() == 0)
        {
            pChannel_->disableWrite();
            Task task(this, shared_from_this());
            loop_->queueInLoop(task);
        }
    }
}

void TcpConnection::handleClose() 
{
    pChannel_->disableRead();
    pServerCallBack_->closeConnection(shared_from_this());
}

void TcpConnection::sendInLoop(const std::string& message)
{
    if(pChannel_->isWriting())
    {
        outBuf_.append(message);
    }
    else
    {
        int writeBytes = ::write(cfd_->fd(), message.c_str(), message.size());
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
                Task task(this, shared_from_this());
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
        Task task(this, message, shared_from_this());
        loop_->runInLoop(task);
    }
}

void TcpConnection::setUser(IMuduoUser* pUser)
{
    pUser_ = pUser;
}

void TcpConnection::setServerCallBack(IServerCallBack* pServerCallBack)
{
    pServerCallBack_ = pServerCallBack;
}

bool TcpConnection::isConnected() const
{
    return status_ == kConnected;
}

bool TcpConnection::isDisconnected() const
{
    return status_ == kDisconnected;
}

void TcpConnection::setStatus(Status status)
{
    status_ = status;
}

void TcpConnection::connectEstablished()
{
    setStatus(kConnected);
    pChannel_->tie(weak_from_this());
    if(pUser_) pUser_->OnConnection(shared_from_this());
}

void TcpConnection::connectDestoryed()
{
    pChannel_->remove();
    setStatus(kDisconnected);
    if(pUser_) pUser_->OnConnection(shared_from_this());
}

int TcpConnection::getSocket()
{
    return cfd_->fd();
}

int TcpConnection::getEventLoopTid()
{
    return loop_->getTid();
}

void TcpConnection::run0(const std::shared_ptr<void>& param)
{
    pUser_->OnWriteComplete(shared_from_this());
}

void TcpConnection::run2(const std::string& message, const std::shared_ptr<void>& param)
{
    sendInLoop(message);
}

