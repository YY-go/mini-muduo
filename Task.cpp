#include "Task.h"
#include "IRun.h"


Task::Task(IRun0* func, const std::shared_ptr<void>& param)
    :func0_(func)
    ,func2_(nullptr)
    ,param_(param)
    ,vparam_(nullptr)
{

}

Task::Task(IRun0* func)
    :func0_(func)
    ,func2_(nullptr)
    ,param_(nullptr)
    ,vparam_(nullptr)
{

}

Task::Task(IRun2* func, const std::string& str, const std::shared_ptr<void>& param)
    :func0_(nullptr)
    ,func2_(func)
    ,str_(str)
    ,param_(param)
    ,vparam_(nullptr)
{

}

Task::Task(IRun2* func, const std::string& str, void* param)
    :func0_(nullptr)
    ,func2_(func)
    ,str_(str)
    ,param_(nullptr)
    ,vparam_(param)
{

}

void Task::doTask()
{
    if(func0_) 
    {
        if(param_)
            func0_->run0(param_);
        else
            func0_->run0();
    }
    if(func2_)
    {
        if(param_)  func2_->run2(str_, param_);
        if(vparam_) func2_->run2(str_, vparam_);
    }
}

