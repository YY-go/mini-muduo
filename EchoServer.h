#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "IMuduoUser.h"
#include "TcpServer.h"
class EventLoop;

class EchoServer : public IMuduoUser
{
public:
    EchoServer(EventLoop* loop);
    ~EchoServer();
    void start();
    virtual void OnMessage(TcpConnection* pCon, std::string* data);
    virtual void OnConnection(TcpConnection* pCon);
private:
    EventLoop* loop_;
    TcpServer Server_;
};

#endif
