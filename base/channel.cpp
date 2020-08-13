#include <iostream>
#include <unistd.h>
#include "channel.h"
using namespace std;


Channel::Channel(EventLoop *loop)
        : loop_(loop), events_(0), lastEvents_(0), fd_(0)
{

}

Channel::Channel(EventLoop *loop, int fd)
        : loop_(loop), events_(0), lastEvents_(0), fd_(fd)
{

}


Channel::~Channel(){
    
}
