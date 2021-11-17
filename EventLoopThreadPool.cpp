#include "EventLoopThreadPool.h"
#include "EventLoopThread.h"


EventLoopThreadPool::EventLoopThreadPool()
    :whichLoop_(0)
{

}

void EventLoopThreadPool::start(int numThreads)
{
    threads_.reserve(numThreads);
    loops_.reserve(numThreads);
    for(int i = 0; i < numThreads; ++i)
    {
        Task task(this);
        EventLoopThread* pThread = new EventLoopThread(task);
        threads_.push_back(pThread);
        EventLoop* loop = pThread->start();
        loops_.push_back(loop);
    }
}

EventLoop* EventLoopThreadPool::getLoop()
{
    EventLoop* loop = nullptr;
    if(!loops_.empty())
    {
        loop = loops_[whichLoop_];
        ++whichLoop_;
        if((size_t)whichLoop_ >= loops_.size())
            whichLoop_ = 0;
    }

    return loop;
}

void EventLoopThreadPool::run0()
{
    runInThead();
}

void EventLoopThreadPool::runInThead()
{
}

