#ifndef tcpconnection_h
#define tcpconnection_h

#include "IChannelCallBack.h"
class Channel;

class TcpConnection : IChannelCallBack
{
public:
    TcpConnection(int epollfd, int sockfd);
    ~TcpConnection();

    virtual void OnIn(int sockfd);
private:
    int epollfd_;
    int cfd_;
    Channel* pChannel_;
};

#endif
