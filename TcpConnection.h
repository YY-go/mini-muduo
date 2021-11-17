#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "IChannelCallBack.h"
#include "Buffer.h"
#include "IRun.h"
class Channel;
class EventLoop;
class IMuduoUser;
class IAcceptorCallback;

class TcpConnection : public IChannelCallBack
                    , public IRun0
                    , public IRun2
{
public:
    TcpConnection(EventLoop* loop, int sockfd);
    ~TcpConnection();
    void send(const std::string& message);
    void sendInLoop(const std::string& message);
    void connectEstablished();
    void setUser(IMuduoUser* pUser);
    int getSocket();
    virtual void handleRead();
    virtual void handleWrite();
    virtual void run0();
    virtual void run2(const std::string& message, void* param);
private:
    EventLoop* loop_;
    int cfd_;
    Channel* pChannel_;
    IMuduoUser* pUser_;
    Buffer inBuf_;
    Buffer outBuf_;
};

#endif
