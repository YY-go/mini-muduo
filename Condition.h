#ifndef CONDITION_H
#define CONDITION_H

#include <pthread.h>
#include "Mutex.h"

class Condition
{
public:
    Condition(MutexLock& mutex)
        :mutex_(mutex)
    {
        pthread_cond_init(&condid_, nullptr);
    }

    ~Condition()
    {
        pthread_cond_destroy(&condid_);
    }

    void wait()
    {
        pthread_cond_wait(&condid_, mutex_.getPthreadMutex());
    }

    void notify()
    {
        pthread_cond_signal(&condid_);
    }

    void notifyAll()
    {
        pthread_cond_broadcast(&condid_);
    }
private:
    MutexLock& mutex_;
    pthread_cond_t condid_;
};


#endif
