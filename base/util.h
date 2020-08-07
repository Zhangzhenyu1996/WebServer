#ifndef UTIL_H
#define UTIL_H


#define PATHLEN 128
#define SIZE 8192

#define DELIMITER "="

#define SUCCESS 0
#define ERROR   -1

#define MIN(a,b) ((a) < (b) ? (a) (b))

typedef struct conf
{
    char root[PATHLEN];
    int port;
    int threadNum;
}Zconf;

int ReadConf(char* filename, Zconf* conf);
int BindListenFd(int port);
int SetSocketNonBlocking(int fd);
void AcceptConnection(int fd, int epoll_fd, char* path);

#endif