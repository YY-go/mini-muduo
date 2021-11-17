#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "IChannelCallBack.h"
#include <vector>
class Channel;
class IRun;
class Epoll;

class EventLoop : public IChannelCallBack
{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void update(Channel* pChannel);
    void queueLoop(IRun* pRun);

    virtual void handleRead();
    virtual void handleWrite();
private:
    void wakeup();
    int createEventfd();
    void doPendingFunctors();

    bool quit_;
    Epoll* poller_;
    int eventfd_;
    Channel* pWakeupChannel_;
    std::vector<IRun*> PendingFunctors_;
};

#endif
