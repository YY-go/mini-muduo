#ifndef ISERVERCALLBACK_H
#define ISERVERCALLBACK_H

#include <memory>
class TcpConnection;

class IServerCallBack
{
public:
    virtual void newConnection(int sockfd) = 0;
    virtual void closeConnection(const std::shared_ptr<TcpConnection>& pCon) = 0;
};

#endif
