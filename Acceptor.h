#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "IChannelCallBack.h"
#include <memory>
class IServerCallBack;
class EventLoop;
class Socket;
class Channel;

class Acceptor : public IChannelCallBack
{
public:
    Acceptor(EventLoop* loop);
    ~Acceptor();

    virtual void handleRead() override;
    virtual void handleWrite() override;
    virtual void handleClose() override;
    void setCallBack(IServerCallBack* pCallBack);
    void start();
private:
    int init();

    EventLoop* loop_;
    std::unique_ptr<Socket> lfd_;
    std::unique_ptr<Channel> pAcceptorChannel;
    IServerCallBack* pCallBack_;
};

#endif
