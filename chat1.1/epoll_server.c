#include "server.h"
#include <string.h>
#include "link_queue.h"
#include "global.h"

int set_fdlimit()
{
    //设置每个进程允许打开的最大文件数
    //这项功能等价于linux终端命令 "ulimit -n 102400"
    struct rlimit rt;
    rt.rlim_max = rt.rlim_cur = MAXCONN;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1)
    {
        perror("setrlimit error");
        return -1;
    }
    return 0;
}
void epoll_server_init(epoll_server *p, int port)
{
    init_signal();
//    if(set_fdlimit() < 0)
//    {
//        exit(1);
//    }
    memset(p->buff,0,BUFSIZE);
    p->listenfd = socket_bind(port);
    start_listen(p->listenfd, MAXWAIT);

    p->epollfd = epoll_create(FDSIZE);
    //添加监听描述符事件
    add_event(p->epollfd,p->listenfd,EPOLLIN);

}
void add_event(int epollfd, int clifd, int status)
{
    struct epoll_event ev;
    ev.events = status;//LT
    ev.data.fd = clifd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, clifd, &ev);
}
//void hand_events(epoll_server *p)
//{
//    while(1)
//    {
//        //获取已经准备好的描述符事件
//        int n = epoll_wait(p->epollfd, p->events, EPOLLEVENTS, -1);
//        for (int i = 0; i < n; i++) //进行选好遍历
//        {
//            int fd = p->events[i].data.fd;
//            //根据描述符的类型和事件类型进行处理
//            if ((fd == p->listenfd) &&(p->events[i].events & EPOLLIN))
//            {
//                handle_accpet(p->epollfd,fd); //处理接受事件
//            }else if (p->events[i].events & EPOLLIN)
//            {
//                do_read(p->epollfd,fd,p->buff);         //处理写事件
//            }else if (p->events[i].events & EPOLLOUT)
//            {
//                do_write(p, fd, p->buff);        //处理写事件
//            }
//        }
//    }

//}
void handle_accpet(int epollfd, int listenfd)
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t  cliaddrlen = sizeof(cliaddr);

    clifd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddrlen);
    if (clifd == -1)
        perror("accpet error:");
    else
    {
        printf("accept a new client: %s:%d,fd=%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port,clifd);
        //添加一个客户描述符和事件
        add_event(epollfd,clifd,EPOLLIN);
    }
}
void do_read(int epollfd, int fd, char *buff)
{
    int n = read(fd, buff, BUFSIZE);
    if (n == -1)
    {
        perror("read error:");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    }
    else if (n == 0)
    {
        fprintf(stderr,"client close,fd=%d\n",fd);
        close(fd);
        delete_event(epollfd,fd,EPOLLIN);
    }
    else
    {
        printf("read message is: %s,fd=%d\n",buff,fd);
        DATATYPE mes;
        mes.fd = fd;
        mes.nbytes = n;
        bzero(mes.mes, MAXBUF);
        strcpy(mes.mes, buff);
        printf("new message enqueue: %s data size:%d\n", mes.mes, n);
        en_queue(&g_queue, mes);
        memset(buff,0,BUFSIZE);
        //修改描述符对应的事件，由读改为写
        modify_event(epollfd, fd, EPOLLOUT);
    }
}
void do_write(epoll_server *p, int fd, char *buff)
{
    int nwrite;
    nwrite = write(fd,buff,strlen(buff));
    if (nwrite == -1)
    {
        perror("write error:");
        close(fd);
        delete_event(p->epollfd,fd,EPOLLOUT);
    }else
    {
        printf("write modify_event(p->epollfd,fd,EPOLLIN);\n");
        modify_event(p->epollfd,fd,EPOLLIN);
    }
    printf("do write :%s\n", buff);
    //memset(buff,0,BUFSIZE);
}
void delete_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}
void modify_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
}
void close_ep_server(epoll_server *p)
{
    close(p->epollfd);
}
