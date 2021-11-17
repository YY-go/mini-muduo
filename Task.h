#ifndef TASK_H
#define TASK_H

#include <string>
class IRun0;
class IRun2;

class Task
{
public:
    Task(IRun0* func);
    Task(IRun2* func, const std::string& str, void* param);
    void doTask();
private:
    IRun0* func0_;
    IRun2* func2_;
    std::string str_;
    void* param_;
};


#endif
