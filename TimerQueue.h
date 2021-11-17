#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include "IChannelCallBack.h"
#include "TimeStamp.h"
#include "IRun.h"
#include <vector>
#include <set>
class EventLoop;
class Channel;

class TimerQueue : public IChannelCallBack
{
public:
    class Timer
    {
    public:
        Timer(MyTimeStamp stamp, IRun* pRun, double interval)
            :stamp_(stamp)
            , id_(stamp)
            , pRun_(pRun)
            , interval_(interval)
        {

        }
        
        MyTimeStamp getStamp()
        {
            return stamp_;
        }

        MyTimeStamp getId()
        {
            return id_;
        }

        void run()
        {
            pRun_->run(this);
        }

        bool isRepeat()
        {
            return interval_ > 0;
        }

        void moveToNext()
        {
            stamp_ = MyTimeStamp::nowAfter(interval_);
        }
    private:
        MyTimeStamp stamp_;
        MyTimeStamp id_;
        IRun* pRun_;
        double interval_;
    };

    class AddTimerWrapper : public IRun
    {
    public:
        AddTimerWrapper(TimerQueue* pQueue)
            : pQueue_(pQueue)
        {
            
        }

        virtual void run(void* param)
        {
            pQueue_->doAddTimer(param);
        }
    private:
        TimerQueue* pQueue_;
    };

    class CancelTimerWrapper : public IRun
    {
    public:
        CancelTimerWrapper(TimerQueue* pQueue)
            :pQueue_(pQueue)
        {
        }
        
        virtual void run(void* param)
        {
            pQueue_->doCancelTimer(param);
        }
    private:
        TimerQueue* pQueue_;
    };

    TimerQueue(EventLoop* loop);
    ~TimerQueue();
    void doAddTimer(void* param);
    void doCancelTimer( void* param );
    int64_t addTimer(IRun* pRun, MyTimeStamp when, double interval);
    void cancelTimer(int64_t timerfd);

    virtual void handleRead();
    virtual void handleWrite();

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
    AddTimerWrapper* pAddTimerWrapper_;
    CancelTimerWrapper* pCancelTimerWrapper_;
};

#endif
