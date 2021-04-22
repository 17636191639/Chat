#include "eventloop.h"
#include <fcntl.h>

EventLoop::EventLoop()
{

}
EventLoop::~EventLoop()
{

}
void EventLoop::runLoop()
{
    while(1)
    {
        handleEvents(waitEvent());
    }

}
void EventLoop::handleEvents(int size)
{
    int fd = 0;
    for(int i = 0; i < size; i++)
    {

        fd = m_events[i].data.fd;
        //根据描述符的类型和事件类型进行处理
        if((isListenFD(fd)) && (m_events[i].events & EPOLLIN))
        {
            m_handerMap[fd]->handleAccept();   //处理新连接,将新的fd跟 socket 加入到map里
                                               //m_handerMap  key: fd  value: 处理函数的函数指针
        }else if(m_events[i].events & EPOLLIN)
        {
            m_handerMap[fd]->handleRead();    //处理读事件   将读取的数据放入读取处理函数 (之后使用线程池替代)
                                              //处理完之后将返回的消息加入到写事件队列, fd的状态由读改为写,等待调用
        }else if(m_events[i].events & EPOLLOUT)
        {
            m_handerMap[fd]->handleWrite();    //
        }
    }
}

//注册监听事件处理函数
bool EventLoop::registerServerEvent(int fd, HandlePtr handler)
{
    m_listenList.push_back(fd);
    m_handerMap[fd] = handler;
    if(addEvent(fd, EPOLLIN))
    {
        setNonBlocking(fd);
        return true;
    }

    printf("Error: %d register Event!\n", fd);
    return false;
}
//注册读写事件处理函数
bool EventLoop::registerSocketEvent(int fd, HandlePtr handler)
{
    m_handerMap[fd] = handler;
    if(addEvent(fd, EPOLLIN))
    {
        setNonBlocking(fd);
        return true;
    }

    printf("Error: %d register Event!\n", fd);
    return false;
}
bool EventLoop::isListenFD(int fd)
{
    for(int i = 0; i < m_listenList.size(); i++)
    {
        if(fd == m_listenList.at(i))
        {
            return true;
        }
    }

    return false;
}
int EventLoop::setNonBlocking(int fd)
{
    int oldOption = fcntl(fd, F_GETFL);
    int newOption = oldOption | O_NONBLOCK;
    fcntl(fd, F_SETFL, newOption);
    return oldOption;
}
