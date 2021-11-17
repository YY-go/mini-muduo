#include <sys/time.h>
#include "TimeStamp.h"
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS

MyTimeStamp::MyTimeStamp(double microSeconds /*= 0.0*/)
    :microSecondsSinceEpoch_(microSeconds)
{

}

MyTimeStamp::~MyTimeStamp()
{

}

bool MyTimeStamp::valid()
{
    return microSecondsSinceEpoch_ > 0;
}

int64_t MyTimeStamp::microSecondsSinceEpoch()
{
    return microSecondsSinceEpoch_;
}

std::string MyTimeStamp::toString() const
{
    char buf[32] = {0};
    int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
    snprintf(buf, sizeof buf - 1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
}

MyTimeStamp MyTimeStamp::now()
{
    return MyTimeStamp(MyTimeStamp::nowMicroSeconds());
}

MyTimeStamp MyTimeStamp::nowAfter(double seconds)
{
    return MyTimeStamp(MyTimeStamp::nowMicroSeconds() + kMicroSecondsPerSecond * seconds);
}

double MyTimeStamp::nowMicroSeconds()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * kMicroSecondsPerSecond + tv.tv_usec;
}

bool operator <(MyTimeStamp l, MyTimeStamp r)
{
    return l.microSecondsSinceEpoch() < r.microSecondsSinceEpoch();
}

bool operator ==(MyTimeStamp l, MyTimeStamp r)
{
    return l.microSecondsSinceEpoch() == r.microSecondsSinceEpoch();
}

