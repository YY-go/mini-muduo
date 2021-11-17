#ifndef IACCEPTORCALLBACK_H
#define IACCEPTORCALLBACK_H

class IAcceptorCallBack
{
public:
    virtual void newConnection(int sockfd){};
};

#endif
