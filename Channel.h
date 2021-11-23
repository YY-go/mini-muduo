#ifndef CHANNEL_H
#define CHANNEL_H

#include <memory>
class IChannelCallBack;
class EventLoop;

class Channel
{
public:
    Channel(EventLoop* loop, int sockfd);
    ~Channel();
    void setCallBack(IChannelCallBack* callBack);
    void handleEvent();
    void handleEventWithGuard();
    void setRevent(int revent);
    void setIndex(int index);
    void enableRead();
    void disableRead();
    void enableWrite();
    void disableWrite();
    void remove();
    bool isWriting();
    int getEvents();
    int getSockfd();
    int getIndex();
    void tie(const std::weak_ptr<void>&);
private:
    void update();

    int sockfd_;
    int event_;
    int revent_;
    int index_;
    std::weak_ptr<void> tie_;
    bool tied_;
    EventLoop* loop_;
    IChannelCallBack* callBack_;
};

#endif
