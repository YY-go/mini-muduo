#ifndef IMUDUOUSER_H
#define IMUDUOUSER_H

#include <string>
class TcpConnection;

class IMuduoUser
{
public:
    virtual void OnConnection(TcpConnection* pCon) = 0;
    virtual void OnMessage(TcpConnection* pCon, std::string* data) = 0;
};

#endif
