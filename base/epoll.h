#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>

#define MAXEVENTS 1024

class Epoll{
public:
    Epoll();
    ~Epoll();
    void epoll_add();
};


#endif