#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "IChannelCallBack.h"
class Channel;
class IAcceptorCallBack;

class Acceptor : public IChannelCallBack
{
public:
    Acceptor(int epollfd);
    ~Acceptor();

    virtual void OnIn(int sockfd);
    void setCallBack(IAcceptorCallBack* pCallBack);
    void start();
private:
    int init();

    int epollfd_;
    int lfd_;
    Channel* pAcceptorChannel;
    IAcceptorCallBack* pCallBack_;
};

#endif
