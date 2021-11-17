#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "IChannelCallBack.h"
#include <vector>
#include "IRun.h"
#include "TimeStamp.h"
class Channel;
class Epoll;
class TimerQueue;

class EventLoop : public IChannelCallBack
{
public:
    class Runner
    {
    public:
        Runner(IRun* r, void* p)
            :pRun_(r)
            ,param_(p)
        { }

        void doRun()
        {
            pRun_->run(param_);
        }
    private:
        IRun* pRun_;
        void* param_;
    };
    EventLoop();
    ~EventLoop();
    void loop();
    void update(Channel* pChannel);
    void queueLoop(IRun* pRun, void* param);
    int64_t runAt(MyTimeStamp when, IRun* pRun);
    int64_t runAfter(double dalay, IRun* pRun);
    int64_t runEvery(double interval, IRun* pRun);
    void cancelTimer(int64_t timerfd);

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
    std::vector<Runner> PendingFunctors_;
    TimerQueue* pTimerQueue_;
};

#endif
