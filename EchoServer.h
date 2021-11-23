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
    virtual void OnMessage(const std::shared_ptr<TcpConnection> &pCon, Buffer* pBuf) override;
    virtual void OnConnection(const std::shared_ptr<TcpConnection> &pCon) override;
    virtual void OnWriteComplete(const std::shared_ptr<TcpConnection> &pCon) override;
    virtual void OnHighWaterMark(const std::shared_ptr<TcpConnection> &pCon) override;
    virtual void run2(const std::string &str, const std::shared_ptr<void>& param) override;
    virtual void run2(const std::string &str, void *param) override {};
private:
    int fib(int n);
    EventLoop* loop_;
    TcpServer server_;
    ThreadPool threadPool_;
    int64_t timer_;
    int index_;
};

#endif

