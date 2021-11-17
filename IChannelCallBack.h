#ifndef ICHANNELCALLBACK_H
#define ICHANNELCALLBACK_H

class IChannelCallBack
{
public:
    virtual void handleRead() = 0;
    virtual void handleWrite() = 0;
};

#endif
