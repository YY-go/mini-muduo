#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "IRun.h"
#include "BlockingQueue.h"
#include <vector>
class Thread;

class ThreadPool : public IRun
{
public:
    ThreadPool();
    void start(int numThreads);
    void addTask(IRun* pRun);

    virtual void run(void* param);
private:
    void runInThead();
    BlockingQueue<IRun*> tasks_;
    std::vector<Thread*> threads_;
};


#endif
