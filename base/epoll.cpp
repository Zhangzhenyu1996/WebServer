#include <iostream>
#include <assert.h>
#include <errno.h>
#include <sys/epoll.h>
#include "epoll.h"

using namespace std;


const int EVENTSNUM = 4096;
const int  EPOLLWAIT_TIME  = 10000;

// typedef shared_ptr<Channel> sp_channel;

Epoll::Epoll() : epollFd_(epoll_create1(EPOLL_CLOEXEC)),
                 events_(EVENTSNUM)
{
    assert(epollFd_ > 0);
}

Epoll::~Epoll()
{
    
}

void Epoll::epollAdd(sp_channel request, int timeout)
{

}
void Epoll::epollMod(sp_channel request, int timeout)
{

}
void Epoll::epollDel(sp_channel request)
{

}