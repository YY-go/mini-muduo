#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <string>
#include "IChannelCallBack.h"
#include "Buffer.h"
#include "IRun.h"
class Channel;
class EventLoop;
class IMuduoUser;
class IAcceptorCallback;

class TcpConnection : public IChannelCallBack
                    , public IRun
{
public:
    TcpConnection(EventLoop* loop, int sockfd);
    ~TcpConnection();
    void send(const std::string& message);
    void connectEstablished();
    void setUser(IMuduoUser* pUser);
    int getSocket();
    virtual void handleRead();
    virtual void handleWrite();
    virtual void run();
private:
    EventLoop* loop_;
    int cfd_;
    Channel* pChannel_;
    IMuduoUser* pUser_;
    Buffer inBuf_;
    Buffer outBuf_;
};

#endif
