#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "IChannelCallBack.h"
#include <vector>
#include "IRun.h"
#include "TimeStamp.h"
#include "Mutex.h"
#include "Task.h"
#include <memory>
class Socket;
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
    void removeChannel(Channel* pChannel);
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
    virtual void handleClose();
private:
    void wakeup();
    int createEventfd();
    void doPendingFunctors();

    bool quit_;
    bool callingPendingFunctors_;
    std::unique_ptr<Epoll> poller_;
    std::unique_ptr<Socket> eventfd_;
    const pid_t threadId_;
    std::unique_ptr<Channel> pWakeupChannel_;
    MutexLock mutex_;
    std::vector<Task> pendingFunctors_;
    std::unique_ptr<TimerQueue> pTimerQueue_;
};

#endif
