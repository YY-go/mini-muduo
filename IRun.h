#ifndef IRUN_H
#define IRUN_H

#include <string>
#include <memory>

class IRun0
{
public:
    virtual void run0(const std::shared_ptr<void>& param) = 0;
    virtual void run0() = 0;
};

class IRun2
{
public:
    virtual void run2(const std::string& str, const std::shared_ptr<void>& param) = 0;
    virtual void run2(const std::string& str, void* param) = 0;
};


#endif
