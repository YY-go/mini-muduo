#ifndef EVENTLOOP_H
#define EVENTLOOP_H
class Channel;
class Epoll;

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void update(Channel* pChannel);
private:
    bool quit_;
    Epoll* poller_;
};

#endif
