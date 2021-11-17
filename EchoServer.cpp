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
    threadPool_.start(3);
}

void EchoServer::OnMessage(TcpConnection* pCon, Buffer* pBuf)
{
    while(pBuf->readableBytes() >= message_length)
    {
        std::string message = pBuf->retrieveAsString(message_length);
        Task task(this, message, pCon);
        threadPool_.addTask(task);
    }
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

void EchoServer::run2(const std::string& str, void* param)
{
    printf("fib(45) = %d, tid = %d\n", fib(45), CurrentThread::tid());
    ((TcpConnection*)param)->send(str + "\n");
}

int EchoServer::fib(int n)
{
    return (n == 1 || n == 2) ? 1 : (fib(n-1) + fib(n-2));
}

