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

void select_server_init(struct select_server *p, int port);
void select_start(struct select_server *p);
void init_signal(void);

int socket_bind(int port);
void fdset_init(struct select_server *p);
void start_listen(int listenfd, int backlog);
int my_select(struct select_server *p);
int is_new_connnect(struct select_server *p);
 int new_connectfd(int listenfd);
 void add_new_connect(struct select_server *p);
 void check_client(struct select_server *p) ;

#endif // SERVER_H
