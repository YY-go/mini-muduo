#ifndef IMUDUOUSER_H
#define IMUDUOUSER_H

#include <memory>
#include <string>
class TcpConnection;
class Buffer;

class IMuduoUser
{
public:
    virtual void OnConnection(const std::shared_ptr<TcpConnection>& pCon) = 0;
    virtual void OnMessage(const std::shared_ptr<TcpConnection>& pCon, Buffer* pBuf) = 0;
    virtual void OnWriteComplete(const std::shared_ptr<TcpConnection>& pCon) = 0;
    virtual void OnHighWaterMark(const std::shared_ptr<TcpConnection>& pCon) = 0;
};

#endif
