#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/epoll.h>
#include "IServerCallBack.h"
#include <map>
#include "EventLoopThreadPool.h"
class Acceptor;
class TcpConnection;
class EventLoop;
class IMuduoUser;
class Channel;
const int max_events = 1024;

class TcpServer : public IServerCallBack
{
public:
    TcpServer(EventLoop* loop);
    ~TcpServer();
    void setCallBack(IMuduoUser* pUser);
    void start();
    virtual void newConnection(int sockfd);
    virtual void closeConnection(const std::shared_ptr<TcpConnection>& pCon);
private:
    EventLoop* loop_;
    struct epoll_event events_[max_events];
    std::map<int, std::shared_ptr<TcpConnection>> connectons_;
    EventLoopThreadPool eventLoopThreadPool_;
    std::unique_ptr<Acceptor> pAcceptor_;
    IMuduoUser* pUser_;
};

#endif
