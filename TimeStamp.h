#ifndef MYTIMESTAMP_H
#define MYTIMESTAMP_H

#include <string>

class MyTimeStamp
{
public:
    MyTimeStamp(double microSeconds = 0.0);
    ~MyTimeStamp();
    bool valid();
    int64_t microSecondsSinceEpoch();
    std::string toString() const;


    static MyTimeStamp now();
    static MyTimeStamp nowAfter(double seconds);
    static double nowMicroSeconds();
    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    int64_t microSecondsSinceEpoch_;
};
bool operator <(MyTimeStamp l, MyTimeStamp r);
bool operator ==(MyTimeStamp l, MyTimeStamp r);

#endif
