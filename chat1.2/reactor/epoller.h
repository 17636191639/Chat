#ifndef EPOLLER_H
#define EPOLLER_H

#include "config.h"
#include <sys/epoll.h>

class Epoller
{
public:
    Epoller();
    ~Epoller();

    //等待事件
    int waitEvent();
    //添加事件
    bool addEvent(int fd,int state);
    //修改事件
    bool modEvent(int fd,int state);
    //删除事件
    bool delEvent(int fd,int state);
protected:
    int m_epollFD;
    struct epoll_event *m_events;
};

#endif // EPOLLER_H
