#include "Thread.h"
#include "IRun.h"
#include <unistd.h>
#include <sys/syscall.h>

void* globalRun(void* arg)
{
    ((Thread*) arg)->run();
    return 0;
}

Thread::Thread(IRun* run)
    :run_(run)
{

}

void Thread::start()
{
    pthread_t t;
    ::pthread_create(&t, nullptr, globalRun, this);
}

void Thread::run()
{
    settid();
    run_->run(nullptr);
}

void Thread::settid()
{
    tid = static_cast<pid_t>(::syscall(SYS_gettid));
}

