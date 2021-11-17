#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H


#include "Task.h"
#include <pthread.h>
#include "Mutex.h"
#include "Condition.h"
class EventLoop;

class EventLoopThread
{
public:
    EventLoopThread(Task& task);
    EventLoop* start();
private:
    void settid();
    void startLoop();
    Task task_;
    pid_t tid_;
    EventLoop* loop_;
    MutexLock mutex_;
    Condition con_;
    friend void* loopGlobalRun(void* arg);
};


#endif
