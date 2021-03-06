#include "ThreadPool.h"
#include "Thread.h"
#include <iostream>
#include "CurrentThread.h"


ThreadPool::ThreadPool()
{

}

void ThreadPool::start(int numThreads)
{
    threads_.reserve(numThreads);
    for(int i = 0; i < numThreads; ++i)
    {
        Task task(this);
        Thread* pThread = new Thread(task);
        threads_.push_back(pThread);
        pThread->start();
    }
}

void ThreadPool::addTask(Task& task)
{
    tasks_.put(task);
}

void ThreadPool::run0()
{
    runInThead();
}

void ThreadPool::runInThead()
{
    std::cout << "worker threadId: " << CurrentThread::tid() << std::endl;
    while(true)
    {
        tasks_.take().doTask();
    }
}

