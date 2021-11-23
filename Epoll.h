#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include <vector>
#include <memory>
class Channel;
class Socket;
const int max_events = 1024;

class Epoll
{
public:
    Epoll();
    ~Epoll();
    void poll(std::vector<Channel*> & pChannels);
    void update(Channel* pChannel);
    void removeChannel(Channel* pChannel);
private:
    std::unique_ptr<Socket> epollfd_;
    struct epoll_event events_[max_events];
};

#endif
