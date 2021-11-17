#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <string>
#include "IChannelCallBack.h"
class Channel;
class EventLoop;
class IMuduoUser;
class IAcceptorCallback;

class TcpConnection : IChannelCallBack
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
private:
    EventLoop* loop_;
    int cfd_;
    Channel* pChannel_;
    IMuduoUser* pUser_;
    std::string* inBuf_;
    std::string* outBuf_;
};

#endif
