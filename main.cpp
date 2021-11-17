#include "TcpServer.h"
#include "EventLoop.h"

int main()
{
    EventLoop eventloop;
    TcpServer tcpServer(&eventloop);
    tcpServer.start();
    eventloop.loop();
    return 0;
}
