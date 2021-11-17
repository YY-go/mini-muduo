#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOOL_H

#include "IRun.h"
#include "Task.h"
#include <vector>
class EventLoopThread;
class EventLoop;

class EventLoopThreadPool : public IRun0
{
public:
    EventLoopThreadPool();
    void start(int numThreads);
    EventLoop* getLoop();

    virtual void run0();
private:
    void runInThead();
    int whichLoop_;
    std::vector<EventLoopThread*> threads_;
    std::vector<EventLoop*> loops_;
};


#endif
