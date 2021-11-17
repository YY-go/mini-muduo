#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/epoll.h>
#include "IChannelCallBack.h"
#include <map>

class Channel;
const int max_events = 1024;

class TcpServer : public IChannelCallBack
{
public:
    TcpServer();
    ~TcpServer();
    void start();
    virtual void OnIn(int sockfd);
private:
    int init();
    void update(Channel* pChannel, int op);

    int epollfd_;
    int lfd_;
    struct epoll_event events_[max_events];
    std::map<int, Channel*> channels_;
};

#endif
