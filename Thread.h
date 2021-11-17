#ifndef THREAD_H
#define THREAD_H


#include "Task.h"
#include <pthread.h>

class Thread
{
public:
    Thread(Task& task);
    void start();
private:
    void settid();
    Task task_;
    pid_t tid;
    friend void* globalRun(void* arg);
};


#endif
