#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "dbg.h"

int ReadConf(char* filename, Zconf* conf)
{   
    FILE* fp = fopen(filename, "r");
    if(!fp)
        return ERROR;
    char buffer[SIZE];
    int bufferLen = SIZE;
    char* currentPos = buffer;
    char* delimiterPos = NULL;
    int i = 0;
    int pos = 0;
    int lineLen = 0;
    while(fgets(currentPos, bufferLen - pos, fp)){
        //get "=" location
        delimiterPos = strstr(currentPos,  DELIMITER);
        if(!delimiterPos)
            return ERROR;
        if(currentPos[strlen(currentPos) - 1] == '\n')
            currentPos[strlen(currentPos) - 1] = '\0';
        //get root msg
        if(strncmp("root", currentPos, 4) == 0){
            delimiterPos = delimiterPos + 1;
            while(*delimiterPos != '#'){
                conf->root[i++] = *delimiterPos;
                ++delimiterPos;
            }
        }
        //get port val
        if(strncmp("port", currentPos, 4) == 0)
            conf->port = atoi(delimiterPos+1);
        
        //get thread num
        if(strncmp("threadNum", currentPos, 9) == 0)
            conf->threadNum = atoi(delimiterPos+1);
        lineLen = strlen(currentPos);
        currentPos += lineLen;
    }
    fclose(fp);
    return SUCCESS;
}

int BindListenFd(int port)
{
    port = ((port < 1024) || (port > 65535)) ? 6666 : port;

    int listenFd = 0;
    //创建socket连接符
    if((listenFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return -1;

    //消除绑定时"address already in use"的错误,重用本地地址和端口
    int optval = 1;
    if(setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR,
                  (const void*)&optval, sizeof(int)) < 0)
        return -1;

    // 设置服务器IP和Port, bind 和 listen函数
    struct sockaddr_in serverAddr;
    memset((char*)&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons((unsigned short)port);
    if(bind(listenFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        return ERROR;
    
    //监听fd，设置最长等待队列
    if(listen(listenFd, 1024) < 0)
        return ERROR;
    
    return listenFd;
}
//设置为非阻塞IO
int SetSocketNonBlocking(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    if(flag == -1){
        log_err("F_GETFL");
        return -1;
    }

    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1){
        log_err("F_SETFL");
        return -1;
    }
    return 0;
}
void AcceptConnection(int fd, int epoll_fd, char* path)
{




}
