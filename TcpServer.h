#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/epoll.h>
#include "IAcceptorCallBack.h"
#include <map>
class Acceptor;
class TcpConnection;

class Channel;
const int max_events = 1024;

class TcpServer : public IAcceptorCallBack
{
public:
    TcpServer();
    ~TcpServer();
    void start();
    virtual void newConnection(int sockfd);
private:
    void update(Channel* pChannel, int op);

    int epollfd_;
    struct epoll_event events_[max_events];
    std::map<int, TcpConnection*> connectons_;
    Acceptor* pAcceptor_;
};

#endif
