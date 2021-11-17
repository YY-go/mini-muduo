#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include "IChannelCallBack.h"
#include "TimeStamp.h"
#include "IRun.h"
#include <vector>
#include <set>
class EventLoop;
class Channel;
class Timer;

class TimerQueue : public IChannelCallBack
                 , public IRun2
{
public:

    TimerQueue(EventLoop* loop);
    ~TimerQueue();
    void doAddTimer(Timer* timer);
    void doCancelTimer(Timer* timer);
    int64_t addTimer(IRun0* pRun, MyTimeStamp when, double interval);
    void cancelTimer(int64_t timerId);

    virtual void handleRead();
    virtual void handleWrite();
    virtual void run2(const std::string& str, void* timer);

    private:
    typedef std::pair<MyTimeStamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;

    int createTimerfd();
    std::vector<TimerQueue::Entry> getExpired(MyTimeStamp now);
    void readTimerfd(int timerfd, MyTimeStamp now);
    void reset(const std::vector<Entry>& expired, MyTimeStamp now);
    void resetTimerfd(int timerfd, MyTimeStamp stamp);
    bool insert(Timer* pItem);
    struct timespec howMuchTimeFromNow(MyTimeStamp when);

    int timerfd_;
    TimerList timers_;
    EventLoop* loop_;
    Channel* timerfdChannel_;
};

#endif
