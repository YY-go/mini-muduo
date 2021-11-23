#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "IChannelCallBack.h"
#include "Buffer.h"
#include "IRun.h"
#include "memory"
class Channel;
class EventLoop;
class IMuduoUser;
class IServerCallBack;
class Socket;

class TcpConnection : public IChannelCallBack
                    , public IRun0
                    , public IRun2
                    , public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop* loop, int sockfd);
    ~TcpConnection();
    void send(const std::string& message);
    void sendInLoop(const std::string& message);
    void connectEstablished();
    void connectDestoryed();
    void setUser(IMuduoUser* pUser);
    void setServerCallBack(IServerCallBack* pServerCallBack);
    int getSocket();
    int getEventLoopTid();
    bool isConnected() const;
    bool isDisconnected() const;
    virtual void handleRead() override;
    virtual void handleWrite() override;
    virtual void handleClose() override;
    virtual void run0(const std::shared_ptr<void>& param) override;
    virtual void run0() override {};
    virtual void run2(const std::string& message, const std::shared_ptr<void>& param) override;
    virtual void run2(const std::string& message, void* param) override {};
private:
    enum Status { kConnecting, kConnected, kDisconnecting, kDisconnected };
    void setStatus(Status status);

    EventLoop* loop_;
    std::unique_ptr<Socket> cfd_;
    std::unique_ptr<Channel> pChannel_;
    IMuduoUser* pUser_;
    IServerCallBack* pServerCallBack_;
    size_t highWaterMark_;
    Status status_;
    Buffer inBuf_;
    Buffer outBuf_;
};

#endif
