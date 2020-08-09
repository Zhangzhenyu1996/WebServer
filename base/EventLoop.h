#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include "noncopyable.h"
#include "epoll.h"
#include "channel.h"
#include "CurrentThread.h"
#include "MutexLock.h"
using namespace std;
/*
基于one loop per thread即一个线程一个事件循环机制,
每个eventLoop就是一个Reactor模型，主要功能是运行事件循环
如等待时间发生然后处理发生的事件
*/
class Channel;
class Epoll;

class EventLoop : noncopyable{
public:
        //实现时间循环
        //实现定时回调，通过timerfd和TimerQueue实现
        //实现用户任务回调,为了线程安全有可能其他线程向IO线程的
        //EventLoop添加任务，此时通过eventfd通知EventLoop执行用户任务
        typedef function<void()> Functor;//回调函数
        EventLoop();
        ~EventLoop();
        //EventLoop主体，用于事件循环，通过poll()获取就绪的事件集合
        //并通过handleEvent()执行就绪事件回调
        void loop();
        void quit();
        void runInLoop(Functor&& cb);
        void queueInLoop(Functor&& cb);
        bool isInLoopThread() const{return threadId_ == CurrentThread::tid();};
        void assertInLoopThread(){
                if(!isInLoopThread())
                   abortNotInLoopThread();
        }
        void wakeup();
        void removeChannel(shared_ptr<Channel> channel);
        void updateChannel(shared_ptr<Channel>  channel, int timeout = 0);
        void addChannel(shared_ptr<Channel>  channel, int timeout = 0);
private:
        bool looping_;
        shared_ptr<Epoll> poller_;
        int wakeupFd_;
        bool quit_;
        bool eventHandling_;
        bool callingPendingFunctors_;
        mutable MutexLock mutex_;
        const pid_t threadId_;
        vector<Functor> pendingFunctors_;
        shared_ptr<Channel> wakeupChannel_;
        void abortNotInLoopThread();
        void handleRead();
        void doPendingFunctors();
        void handleConn();
};



#endif