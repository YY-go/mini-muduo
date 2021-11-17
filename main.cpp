#include "EchoServer.h"
#include "EventLoop.h"

int main()
{
    EventLoop eventloop;
    EchoServer echoserver(&eventloop);
    echoserver.start();
    eventloop.loop();
    return 0;
}
