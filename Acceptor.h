#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "IChannelCallBack.h"
class Channel;
class IAcceptorCallBack;
class EventLoop;

class Acceptor : public IChannelCallBack
{
public:
    Acceptor(EventLoop* loop);
    ~Acceptor();

    virtual void OnIn(int sockfd);
    void setCallBack(IAcceptorCallBack* pCallBack);
    void start();
private:
    int init();

    EventLoop* loop_;
    int lfd_;
    Channel* pAcceptorChannel;
    IAcceptorCallBack* pCallBack_;
};

#endif
