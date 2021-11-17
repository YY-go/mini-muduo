#ifndef TIMER_H
#define TIMER_H

#include "TimeStamp.h"
#include "IRun.h"

class Timer
{
public:
    Timer(MyTimeStamp stamp, IRun0* pRun, double interval)
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

    void timeout()
    {
        pRun_->run0();
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
    IRun0* pRun_;
    double interval_;
};


#endif
