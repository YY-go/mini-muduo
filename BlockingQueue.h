#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <deque>
#include "Mutex.h"
#include "Condition.h"


template<typename T>
class BlockingQueue
{
public:
    BlockingQueue()
        :cond_(mutex_)
    {
    }

    void put(const T& one)
    {
        MutexLockGuard lock(mutex_);
        queue_.push_back(one);
        cond_.notify();
    }

    T take()
    {
        MutexLockGuard lock(mutex_);
        while(queue_.empty())
        {
            cond_.wait();
        }
        T front(queue_.front());
        queue_.pop_front();
        return front;
    }
private:
    std::deque<T> queue_;
    MutexLock mutex_;
    Condition cond_;
};


#endif
