#ifndef SOCKET_H
#define SOCKET_H

#include <unistd.h>

class Socket
{
public:
    Socket(int sockfd) :sockfd_(sockfd)
    {}
    ~Socket()
    { ::close(sockfd_); }
    int fd()
    { return sockfd_; }
private:
    int sockfd_;
};


#endif
