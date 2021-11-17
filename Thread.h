#ifndef THREAD_H
#define THREAD_H


#include <pthread.h>
class IRun;

class Thread
{
public:
    Thread(IRun* run);
    void start();
    void run();
private:
    void settid();
    IRun* run_;
    pid_t tid;
};


#endif
