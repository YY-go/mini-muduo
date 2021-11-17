#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "IMuduoUser.h"
#include "TcpServer.h"
#include "IRun.h"
#include "ThreadPool.h"
class EventLoop;

class EchoServer : public IMuduoUser
                 , public IRun2
{
public:
    EchoServer(EventLoop* loop);
    ~EchoServer();
    void start();
    virtual void OnMessage(TcpConnection* pCon, Buffer* pBuf);
    virtual void OnConnection(TcpConnection* pCon);
    virtual void OnWriteComplete(TcpConnection* pCon);
    virtual void run2(const std::string& str, void* tcp);
private:
    int fib(int n);
    EventLoop* loop_;
    TcpServer server_;
    ThreadPool threadPool_;
    int64_t timer_;
    int index_;
};

#endif

