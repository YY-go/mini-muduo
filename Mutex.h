#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

class MutexLock
{
public:
    MutexLock()
    {
        pthread_mutex_init(&mutexid_, nullptr);
    }

    ~MutexLock()
    {
        pthread_mutex_destroy(&mutexid_);
    }

    void lock()
    {
        pthread_mutex_lock(&mutexid_);
    }

    void unlock()
    {
        pthread_mutex_unlock(&mutexid_);
    }

    pthread_mutex_t* getPthreadMutex()
    {
        return &mutexid_;
    }

private:
    pthread_mutex_t mutexid_;
};


class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock& mutex)
        : mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexLockGuard()
    {
        mutex_.unlock();
    }

private:
    MutexLock& mutex_;
};

#endif
