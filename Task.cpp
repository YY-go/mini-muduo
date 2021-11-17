#include "Task.h"
#include "IRun.h"


Task::Task(IRun0* func)
    :func0_(func)
    ,func2_(nullptr)
    ,param_(nullptr)
{

}

Task::Task(IRun2* func, const std::string& str, void* param)
    :func0_(nullptr)
    ,func2_(func)
    ,str_(str)
    ,param_(param)
{

}

void Task::doTask()
{
    if(func0_) func0_->run0();
    if(func2_) func2_->run2(str_, param_);
}

