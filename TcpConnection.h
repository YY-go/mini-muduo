#ifndef tcpconnection_h
#define tcpconnection_h

#include "IChannelCallBack.h"
class Channel;
class EventLoop;

class TcpConnection : IChannelCallBack
{
public:
    TcpConnection(EventLoop* loop, int sockfd);
    ~TcpConnection();

    virtual void OnIn(int sockfd);
private:
    EventLoop* loop_;
    int cfd_;
    Channel* pChannel_;
};

#endif
