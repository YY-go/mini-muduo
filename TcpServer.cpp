#include "TcpServer.h"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include "Channel.h"
#include <vector>
#include "Acceptor.h"
#include "TcpConnection.h"
#include <signal.h>

using namespace std;

TcpServer::TcpServer(EventLoop* loop)
    : loop_(loop)
    , pAcceptor_(new Acceptor(loop_))
    , pUser_(nullptr)
{
    ::signal(SIGPIPE, SIG_IGN);
}

TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
    pAcceptor_->setCallBack(this);
    pAcceptor_->start();
    eventLoopThreadPool_.start(2);
}

void TcpServer::newConnection(int sockfd)
{

    EventLoop* loop = eventLoopThreadPool_.getLoop();
    if(loop == nullptr) loop = loop_;
    std::shared_ptr<TcpConnection> pTcpConnecton(new TcpConnection(loop, sockfd));
    connectons_[sockfd] = pTcpConnecton;
    pTcpConnecton->setUser(pUser_);
    pTcpConnecton->setServerCallBack(this);
    pTcpConnecton->connectEstablished();
}

void TcpServer::closeConnection(const std::shared_ptr<TcpConnection>& pCon)
{
    connectons_.erase(pCon->getSocket());
    pCon->connectDestoryed();
}

void TcpServer::setCallBack(IMuduoUser* pUser)
{
    pUser_ = pUser;
}

