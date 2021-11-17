#ifndef CHANNEL_H
#define CHANNEL_H

class IChannelCallBack;

class Channel
{
public:
    Channel(int epollfd, int sockfd);
    ~Channel();
    void setCallBack(IChannelCallBack* callBack);
    void handleEvent();
    void setRevent(int revent);
    void enableRead();
private:
    void update();

    int epollfd_;
    int sockfd_;
    int event_;
    int revent_;
    IChannelCallBack* callBack_;
};

#endif
