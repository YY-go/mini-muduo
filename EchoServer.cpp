#include "EchoServer.h"
#include <iostream>
#include "TcpConnection.h"

const int message_length = 8;

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

void EchoServer::OnMessage(TcpConnection* pCon, std::string* data)
{
    while(data->size() > message_length)
    {
        std::string message = data->substr(0, message_length);
        *data = data->substr(message_length, data->size());
        pCon->send(message + "\n");
    }
}

void EchoServer::OnConnection(TcpConnection* pCon)
{
    std::cout << "new connection, socket: " << pCon->getSocket() << std::endl;
}

