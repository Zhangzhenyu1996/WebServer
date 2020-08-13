#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <bits/sigaction.h>
#include "util.h"
#include "dbg.h"
#include "EventLoop.h"
#define CONF "../../init.conf"

char* confFile = (char*)CONF;
Zconf conf;

int main()
{
    printf("111111\n");
    //read conf
    assert(ReadConf(confFile, &conf) != -1);
    printf("root:%s, port:%d, threadNum:%d\n", conf.root, conf.port, conf.threadNum);
    
    //处理SIGPIPE信号,如果客户端关闭套接字close,
    //而服务器调用了一次write，服务器会结构到一个RST segment，
    //如果服务器再次调用了write，这个时候会产生SIGPIPE信号，终止程序
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if(sigaction(SIGPIPE, &sa, NULL)){
        log_err("install sigal handler for SIGPIPE failed");
        return 0;
    }


    //监听套接字
    int listenFd = BindListenFd(conf.port);
    
    //设置为非阻塞IO
    int rc = SetSocketNonBlocking(listenFd);
    check(rc == 0, "set socket non blocking");

    //创建epoll和注册监听fd
    // EventLoop mainLoop;

    return 0;
}
