#ifndef CHANNEL_H
#define CHANNEL_H

class IChannelCallBack;
class EventLoop;

class Channel
{
public:
    Channel(EventLoop* loop, int sockfd);
    ~Channel();
    void setCallBack(IChannelCallBack* callBack);
    void handleEvent();
    void setRevent(int revent);
    void enableRead();
    int getEvents();
    int getSockfd();
private:
    void update();

    int sockfd_;
    int event_;
    int revent_;
    EventLoop* loop_;
    IChannelCallBack* callBack_;
};

#endif
