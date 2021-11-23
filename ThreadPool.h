#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "IRun.h"
#include "Task.h"
#include "BlockingQueue.h"
#include <vector>
class Thread;

class ThreadPool : public IRun0
{
public:
    ThreadPool();
    void start(int numThreads);
    void addTask(Task& task);

    virtual void run0() override;
    virtual void run0(const std::shared_ptr<void> &param) override {};
private:
    void runInThead();
    BlockingQueue<Task> tasks_;
    std::vector<Thread*> threads_;
};


#endif
