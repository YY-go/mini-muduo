#ifndef ICHANNELCALLBACK_H
#define ICHANNELCALLBACK_H

class IChannelCallBack
{
public:
    virtual void OnIn(int sockfd) = 0;
};

#endif
