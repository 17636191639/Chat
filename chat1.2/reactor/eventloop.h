#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "epoller.h"
#include <map>
#include <handler.h>
#include <vector>

class EventLoop : public Epoller
{
public:
    EventLoop();
    ~EventLoop();

    //IO多路复用epoll
    void runLoop();

    int setNonBlocking(int fd);
    bool registerServerEvent(int fd, HandlePtr handler);
    bool registerSocketEvent(int fd, HandlePtr handler);

private:
    std::vector<int> m_listenList;
    bool isListenFD(int fd);

    //事件处理函数
    HandleMap m_handerMap;
    void handleEvents(int size);
};

#endif // EVENTLOOP_H
