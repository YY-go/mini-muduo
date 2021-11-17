#include "Acceptor.h"
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <strings.h>
#include "Channel.h"
#include "IAcceptorCallBack.h"


Acceptor::Acceptor(EventLoop* loop)
    : loop_(loop)
    , lfd_(-1)
    , pAcceptorChannel(nullptr)
    , pCallBack_(nullptr)
{

}

Acceptor::~Acceptor()
{

}

int Acceptor::init()
{
    int lfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if(lfd == -1)   perror("socket error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int on = 1;
    setsockopt(lfd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof on);
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof on);

    int ret;
    ret = bind(lfd, (struct sockaddr*) &serv_addr, sizeof serv_addr);
    if(ret == -1)   perror("bind error");

    ret = listen(lfd, 1024);
    if(ret == -1)   perror("listen error");

    return lfd;
}

void Acceptor::start()
{
    lfd_ = init();
    pAcceptorChannel = new Channel(loop_, lfd_);
    pAcceptorChannel->setCallBack(this);
    pAcceptorChannel->enableRead();
}

void Acceptor::setCallBack(IAcceptorCallBack* pCallBack)
{
    pCallBack_ = pCallBack;
}

void Acceptor::handleRead()
{
    struct sockaddr_in clet_addr;
    bzero(&clet_addr, sizeof clet_addr);
    socklen_t clet_addr_len = sizeof clet_addr;

    int cfd = accept4(lfd_, (struct sockaddr*) &clet_addr, &clet_addr_len, SOCK_CLOEXEC | SOCK_NONBLOCK);
    if(cfd == -1)   perror("accept error");
    pCallBack_->newConnection(cfd);
}

void Acceptor::handleWrite()
{

}

