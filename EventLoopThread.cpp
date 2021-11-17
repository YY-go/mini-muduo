#include "EventLoopThread.h"
#include "CurrentThread.h"
#include "IRun.h"
#include <unistd.h>
#include <sys/syscall.h>
#include "TcpConnection.h"
#include "EventLoop.h"
#include "iostream"


void* loopGlobalRun(void* arg)
{
    ((EventLoopThread*)arg)->settid();
    ((EventLoopThread*)arg)->task_.doTask();
    ((EventLoopThread*)arg)->startLoop();
    return 0;
}

EventLoopThread::EventLoopThread(Task& task)
    :task_(task)
    ,loop_(nullptr)
    ,con_(mutex_)
{

}

EventLoop* EventLoopThread::start()
{
    pthread_t t;
    ::pthread_create(&t, nullptr, loopGlobalRun, this);

    EventLoop* loop = nullptr;
    {
        MutexLockGuard lock(mutex_);
        while(loop_ == nullptr)
            con_.wait();
        loop = loop_;
    }

    std::cout << "looptid: " << tid_ << std::endl;
    return loop;
}

void EventLoopThread::settid()
{
    tid_ = CurrentThread::tid(); 
}

void EventLoopThread::startLoop()
{
    EventLoop loop;

    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop;
        con_.notify();
    }
    loop.loop();
}

