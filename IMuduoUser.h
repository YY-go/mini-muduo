#ifndef IMUDUOUSER_H
#define IMUDUOUSER_H

#include <string>
class TcpConnection;
class Buffer;

class IMuduoUser
{
public:
    virtual void OnConnection(TcpConnection* pCon) = 0;
    virtual void OnMessage(TcpConnection* pCon, Buffer* pBuf) = 0;
    virtual void OnWriteComplete(TcpConnection* pCon) = 0;
};

#endif
