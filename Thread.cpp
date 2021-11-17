#include "Thread.h"
#include "CurrentThread.h"
#include "IRun.h"
#include <unistd.h>
#include <sys/syscall.h>
#include "TcpConnection.h"

namespace CurrentThread
{
    __thread int t_cachedTid = 0;
}

void* globalRun(void* arg)
{
    ((Thread*)arg)->settid();
    ((Thread*)arg)->task_.doTask();
    return 0;
}

Thread::Thread(Task& task)
    :task_(task)
{

}

void Thread::start()
{
    pthread_t t;
    ::pthread_create(&t, nullptr, globalRun, this);
}

void Thread::settid()
{
    tid = CurrentThread::tid(); 
}

