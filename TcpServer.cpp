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

using namespace std;

TcpServer::TcpServer(EventLoop* loop)
    : loop_(loop)
    , pAcceptor_(nullptr)
    , pUser_(nullptr)
{

}

TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
    pAcceptor_ = new Acceptor(loop_);
    pAcceptor_->setCallBack(this);
    pAcceptor_->start();
    eventLoopThreadPool_.start(2);
}

void TcpServer::newConnection(int sockfd)
{

    EventLoop* loop = eventLoopThreadPool_.getLoop();
    if(loop == nullptr) loop = loop_;
    TcpConnection* pTcpConnecton = new TcpConnection(loop, sockfd);
    connectons_[sockfd] = pTcpConnecton;
    pTcpConnecton->setUser(pUser_);
    pTcpConnecton->connectEstablished();
}

void TcpServer::setCallBack(IMuduoUser* pUser)
{
    pUser_ = pUser;
}

