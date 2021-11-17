#include "EchoServer.h"
#include <iostream>
#include "TcpConnection.h"

EchoServer::EchoServer(EventLoop* loop) : loop_(loop), Server_(loop)
{
    Server_.setCallBack(this);
}

EchoServer::~EchoServer()
{

}

void EchoServer::start()
{
    Server_.start();
}

void EchoServer::OnMessage(TcpConnection* pCon, const std::string& data)
{
    std::cout << "onmessage" << std::endl;
    pCon->send(data);
}

void EchoServer::OnConnection(TcpConnection* pCon)
{
    std::cout << "new connection, socket: " << pCon->getSocket() << std::endl;
}



