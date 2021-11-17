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
    void setIndex(int index);
    void enableRead();
    void enableWrite();
    void disableWrite();
    bool isWriting();
    int getEvents();
    int getSockfd();
    int getIndex();
private:
    void update();

    int sockfd_;
    int event_;
    int revent_;
    int index_;
    EventLoop* loop_;
    IChannelCallBack* callBack_;
};

#endif
