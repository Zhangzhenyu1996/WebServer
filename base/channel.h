// #ifndef CHANNEL_H
// #define CHANNEL_H

#pragma once
#include <sys/epoll.h>
#include <iostream>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "noncopyable.h"
#include "EventLoop.h"

class EventLoop;

class Channel{
public:
     Channel(EventLoop* loop);
     Channel(EventLoop* loop, int fd);
     ~Channel();
private:
    typedef std::function<void()> CallBack;
    EventLoop *loop_;
    int fd_;
    uint32_t events_;
    uint32_t revents_;
    uint32_t lastEvents_;
};

// typedef std::shared_ptr<Channel> sp_channel;


// #endif