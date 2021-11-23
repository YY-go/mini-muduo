#include "EchoServer.h"
#include <iostream>
#include "TcpConnection.h"
#include "sys/time.h"
#include "EventLoop.h"
#include "Task.h"
#include "CurrentThread.h"

const int message_length = 8;

EchoServer::EchoServer(EventLoop* loop)
    : loop_(loop)
    , server_(loop)
    , timer_(-1)
    , index_(0)
{
    server_.setCallBack(this);
}

EchoServer::~EchoServer()
{

}

void EchoServer::start()
{
    server_.start();
    threadPool_.start(4);
}

void EchoServer::OnMessage(const std::shared_ptr<TcpConnection> &pCon, Buffer* pBuf)
{
    while(pBuf->readableBytes() >= message_length)
    {
        std::string message = pBuf->retrieveAsString(message_length);
        Task task(this, message, pCon);
        threadPool_.addTask(task);
    }
}

void EchoServer::OnConnection(const std::shared_ptr<TcpConnection> &pCon)
{
    if(pCon->isConnected()) std::cout << "new connection, socket: " << pCon->getSocket() << ", " << "add to loop thread: " << pCon->getEventLoopTid() << std::endl;
    if(pCon->isDisconnected()) std::cout << "connection close, socket: " << pCon->getSocket() << std::endl;
}

void EchoServer::OnWriteComplete(const std::shared_ptr<TcpConnection> &pCon)
{
    std::cout << "loop thread " << CurrentThread::tid() << " OnWriteComplete" << std::endl;
}

void EchoServer::OnHighWaterMark(const std::shared_ptr<TcpConnection> &pCon) 
{

}

void EchoServer::run2(const std::string& str, const std::shared_ptr<void>& param)
{
    printf("worker thread %d, fib(45) = %d\n", CurrentThread::tid(), fib(45));
    ((TcpConnection*)param.get())->send(str + "\n");
}

int EchoServer::fib(int n)
{
    return (n == 1 || n == 2) ? 1 : (fib(n-1) + fib(n-2));
}

