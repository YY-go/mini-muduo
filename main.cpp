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

using namespace std;

int init()
{
    int lfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if(lfd == -1)
    {
        perror("socket error");
        exit(1);
    }

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
    if(ret == -1)
    {
        perror("bind error");
        exit(1);
    }

    ret = listen(lfd, 1024);
    if(ret == -1)
    {
        perror("listen error");
        exit(1);
    }

    return lfd;
}

int main()
{
    int lfd = init();
    int efd = epoll_create1(EPOLL_CLOEXEC);
    if(efd == -1)
    {
        perror("epoll_create1 error");
        exit(1);
    }

    int ret;
    struct epoll_event ev;
    ev.data.fd = lfd;
    ev.events = EPOLLIN;
    ret = epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &ev);

    const int MAX_EVENTS = 1024;
    struct epoll_event events[MAX_EVENTS];
    while(1)
    {
        int fds = epoll_wait(efd, events, MAX_EVENTS, -1);
        if(fds == -1)
        {
            perror("epoll_wait error");
            exit(1);
        }

        struct sockaddr_in clet_addr;
        bzero(&clet_addr, sizeof clet_addr);
        socklen_t clet_addr_len = sizeof clet_addr;

        for(int i = 0; i < fds; ++i)
        {
            if(events[i].data.fd == lfd)
            {
                int cfd = accept4(lfd, (struct sockaddr*) &clet_addr, &clet_addr_len, SOCK_CLOEXEC | SOCK_NONBLOCK);
                if(cfd == -1)
                {
                    perror("accept error");
                    exit(1);
                }

                ev.data.fd = cfd;
                ev.events = EPOLLIN;
                ret = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &ev);
                if(ret == -1)
                {
                    perror("epoll_ctl error");
                    exit(1);
                }

                cout << "new connection, socket: " << cfd << endl;
            }
            else
            {
                if(events[i].events & EPOLLIN)
                {
                    char buf[1024];
                    ret = read(events[i].data.fd, buf, 1024);
                    if(ret < 0)
                    {
                        perror("read error");
                        cout << "connection close, socket: " << events[i].data.fd << endl;
                        exit(1);
                    }
                    else if(ret == 0)
                    {
                        close(events[i].data.fd);
                        cout << "connection close, socket: " << events[i].data.fd << endl;
                    }
                    else
                    {
                        ret = write(events[i].data.fd, buf, ret);
                        if(ret < 0)
                        {
                            perror("write error");
                            exit(1);
                        }
                    }
                }
            }
        }
    }
    return 0;
}
