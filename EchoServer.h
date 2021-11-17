#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "IMuduoUser.h"
#include "TcpServer.h"
#include "IRun.h"
class EventLoop;

class EchoServer : public IMuduoUser
                 , public IRun
{
public:
    EchoServer(EventLoop* loop);
    ~EchoServer();
    void start();
    virtual void OnMessage(TcpConnection* pCon, Buffer* pBuf);
    virtual void OnConnection(TcpConnection* pCon);
    virtual void OnWriteComplete(TcpConnection* pCon);
    virtual void run(void* param);
private:
    EventLoop* loop_;
    TcpServer Server_;
    int64_t timer_;
    int index_;
};

#endif
