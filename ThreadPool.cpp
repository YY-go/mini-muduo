#include "ThreadPool.h"
#include "Thread.h"


ThreadPool::ThreadPool()
{

}

void ThreadPool::start(int numThreads)
{
    threads_.reserve(numThreads);
    for(int i = 0; i < numThreads; ++i)
    {
        Thread* pThread = new Thread(this);
        threads_.push_back(pThread);
        pThread->start();
    }
}

void ThreadPool::addTask(IRun* pRun)
{
    tasks_.put(pRun);
}

void ThreadPool::run(void* param)
{
    runInThead();
}

void ThreadPool::runInThead()
{
    while(true)
    {
        IRun* task = (IRun*)tasks_.take();
        task->run(nullptr);
    }
}

