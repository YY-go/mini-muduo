#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "IChannelCallBack.h"
#include <vector>
#include "IRun.h"
#include "TimeStamp.h"
#include "Mutex.h"
#include "Task.h"
class Channel;
class Epoll;
class TimerQueue;

class EventLoop : public IChannelCallBack
{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void update(Channel* pChannel);
    void queueInLoop(Task& task);
    void runInLoop(Task& task);
    int64_t runAt(MyTimeStamp when, IRun0* pRun);
    int64_t runAfter(double dalay, IRun0* pRun);
    int64_t runEvery(double interval, IRun0* pRun);
    void cancelTimer(int64_t timerId);
    bool isInLoopThread();
    pid_t getTid();

    virtual void handleRead();
    virtual void handleWrite();
private:
    void wakeup();
    int createEventfd();
    void doPendingFunctors();

    bool quit_;
    bool callingPendingFunctors_;
    Epoll* poller_;
    int eventfd_;
    const pid_t threadId_;
    Channel* pWakeupChannel_;
    MutexLock mutex_;
    std::vector<Task> pendingFunctors_;
    TimerQueue* pTimerQueue_;
};

#endif
