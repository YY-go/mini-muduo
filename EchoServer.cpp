#include "EchoServer.h"
#include <iostream>
#include "TcpConnection.h"
#include "sys/time.h"
#include "EventLoop.h"

const int message_length = 8;

EchoServer::EchoServer(EventLoop* loop)
    : loop_(loop)
    , Server_(loop)
    , timer_(-1)
    , index_(0)
{
    Server_.setCallBack(this);
}

EchoServer::~EchoServer()
{

}

void EchoServer::start()
{
    Server_.start();
}

void EchoServer::OnMessage(TcpConnection* pCon, Buffer* pBuf)
{
    while(pBuf->readableBytes() >= message_length)
    {
        std::string message = pBuf->retrieveAsString(message_length);
        pCon->send(message + "\n");
    }
    timer_ = loop_->runEvery(0.5, this);
}

void EchoServer::OnConnection(TcpConnection* pCon)
{
    std::cout << "new connection, socket: " << pCon->getSocket() << std::endl;
}

void EchoServer::OnWriteComplete(TcpConnection* pCon)
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    std::cout << tv.tv_sec << "." << tv.tv_usec  << " OnWriteComplete" << std::endl;
}

void EchoServer::run(void* param)
{
    std::cout << index_ << std::endl;
    if(index_++ == 3)
    {
        loop_->cancelTimer(timer_);
        index_ = 0;
    }
}
