// #ifndef EPOLL_H
// #define EPOLL_H
#pragma once
#include <sys/epoll.h>
#include <memory>
#include <vector>
#include "channel.h"

class Channel;

typedef std::shared_ptr<Channel> sp_channel;

class Epoll{
public:
    Epoll();
    ~Epoll();
    void epollAdd(sp_channel request, int timeout);
    void epollMod(sp_channel request, int timeout);
    void epollDel(sp_channel request);
    std::vector<std::shared_ptr<Channel> > poll();
    std::vector<std::shared_ptr<Channel> > getEventsRequest(int eventNum);
    void addTimer(std::shared_ptr<Channel> request, int timeout);
    int getEpollFd() {return epollFd_;}
    void handleExpired();
private:
    static const int maxFds = 100000;
    int epollFd_;
    std::vector<epoll_event> events_;
    std::shared_ptr<Channel> fd2chan_[maxFds];
    // std::shared_ptr<HttpData> fd2http_[MAXFDS];
    // TimerManager timerManager_;
};

// #endif