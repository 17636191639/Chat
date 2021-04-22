#include "epoller.h"
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

Epoller::Epoller()
{
    m_epollFD = epoll_create(FDSIZE);
    m_events = new struct epoll_event[EPOLLEVENTS];
}
Epoller::~Epoller()
{
    delete []m_events;
    close(m_epollFD);
}

//等待事件
int Epoller::waitEvent()
{
    return epoll_wait(m_epollFD, m_events, EPOLLEVENTS, -1);   //-1 为阻塞
}
//添加事件
bool Epoller::addEvent(int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;      //LT
    ev.data.fd = fd;
    return !epoll_ctl(m_epollFD, EPOLL_CTL_ADD, fd, &ev);
}
//修改事件
bool Epoller::modEvent(int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    return !epoll_ctl(m_epollFD, EPOLL_CTL_MOD, fd, &ev);
}
//删除事件
bool Epoller::delEvent(int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    return !epoll_ctl(m_epollFD, EPOLL_CTL_DEL, fd, &ev);
}
