#ifndef TASK_H
#define TASK_H

#include <string>
#include <memory>
class IRun0;
class IRun2;

class Task
{
public:
    Task(IRun0* func, const std::shared_ptr<void>& param);
    Task(IRun0* func);
    Task(IRun2* func, const std::string& str, const std::shared_ptr<void>& param);
    Task(IRun2* func, const std::string& str, void* param);
    void doTask();
private:
    IRun0* func0_;
    IRun2* func2_;
    std::string str_;
    std::shared_ptr<void> param_;
    void* vparam_;
};


#endif
