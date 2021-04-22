#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/resource.h>    /*setrlimit */
#include <signal.h>
#include <fcntl.h>
#include <sys/epoll.h>


#define MAXCONN 100000
#define EPOLLEVENTS 60000
#define MAXWAIT 200   //同时等待连接数大小
#define FDSIZE      1024
#define BUFSIZE 1024
struct select_server
{
    int listenfd;
    int nready;   //记录有消息的文件描述符个数
    fd_set allset;//描述词组
    fd_set rset;
    int maxfd;
    int maxi;     //用来记录数组里的可用数据，不必每次都遍历整个数组
    int client[FD_SETSIZE];
    int port;
};
typedef struct epoll_server
{
    int listenfd;
    int epollfd;
    int port;
    struct epoll_event events[EPOLLEVENTS];
    char buff[BUFSIZE];
}epoll_server;

//general
int set_fdlimit();
void init_signal(void);
int socket_bind(int port);
void start_listen(int listenfd, int backlog);
//select
void select_server_init(struct select_server *p, int port);
void select_start(struct select_server *p);
void fdset_init(struct select_server *p);
int my_select(struct select_server *p);
int is_new_connnect(struct select_server *p);
int new_connectfd(int listenfd);
void add_new_connect(struct select_server *p);
void check_client(struct select_server *p) ;
//epoll
void epoll_server_init(epoll_server *p, int port);
void add_event(int epollfd, int clifd, int status);

void handle_accpet(int epollfd, int listenfd);
void do_read(int epollfd,int listenfd, char *buff);
void do_write(epoll_server *p, int fd, char *buff);
void delete_event(int epollfd,int fd,int state);
void modify_event(int epollfd,int fd,int state);
void close_ep_server(epoll_server *p);
#endif // SERVER_H
