#include <iostream>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include "EventLoop.h"
#include "dbg.h"
using namespace std;

__thread EventLoop* t_loopInThisThread = 0;


int createEventfd(){
    int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(evtfd < 0){
        log_err("failed in eventfd");
        abort();
    }
    return evtfd;
}


EventLoop::EventLoop()
    : looping_(false),
       poller_(new Epoll()),
       wakeupFd_(createEventfd()),
       quit_(false),
       eventHandling_(false),
       callingPendingFunctors_(false),
       threadId_(CurrentThread::tid()),
       wakeupChannel_(new Channel(this, wakeupFd_))
{
    log_info("eventloop create", threadId_);
    if(t_loopInThisThread){
        cout << "another eventloop" << t_loopInThisThread
             << "exists in this thread" << threadId_;
    }else{
        t_loopInThisThread = this;
    }

}


void EventLoop::handleConn(){

}

EventLoop::~EventLoop(){

}



void EventLoop::wakeup(){

}



void EventLoop::handleRead(){

}


void EventLoop::runInLoop(Functor&& cb){

}

void EventLoop::queueInLoop(Functor&& cb){

}

void EventLoop::loop(){

}


void EventLoop::doPendingFunctors(){

}


void EventLoop::quit(){
    
}