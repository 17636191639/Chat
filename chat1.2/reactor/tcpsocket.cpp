#include "tcpsocket.h"

#include <string.h>
#include <unistd.h>
#include <iostream>

TcpSocket::TcpSocket(EventLoop *loop, int fd, ThreadPool *pool)
{
    m_fd = fd;
    m_evLoop = loop;
    m_pool = pool;

    bzero(m_recvBuff, BUFF_SIZE);
    bzero(m_sendBuff, BUFF_SIZE);
}

TcpSocket::~TcpSocket()
{
    close(m_fd);
}
void TcpSocket::setRecvDataHandle(recvCallback callbackFun)
{
    std::cout << "setRecvDataHandle" << std::endl;

    m_recvCallbackFun = callbackFun;
}
//void TcpSocket::recvData(char *msg)
//{
////    printf("Recv message is: %s,fd=%d\n", msg, m_fd);
////    for(int i = 0; i < strlen(msg); i++)
////    {
////        if((msg[i] > ('a'-1)) && (msg[i] < ('z'+1)))
////        {
////            msg[i] -= 32;
////        }
////    }
//    m_recvCallbackFun(msg);

//    sendData(msg);
//}

void TcpSocket::sendData(const char *msg)
{
    m_sendQueue.push(msg);
    m_evLoop->modEvent(m_fd,EPOLLOUT);
}

void TcpSocket::handleRead()
{
    bzero(m_recvBuff, BUFF_SIZE);
    int res = read(m_fd, m_recvBuff, BUFF_SIZE);
    if(res == -1)
    {
        perror("read error:");
        close(m_fd);
        m_evLoop->delEvent(m_fd,EPOLLIN);
    }else if(res == 0)
    {
        fprintf(stderr,"client close,fd=%d\n",m_fd);
        close(m_fd);
        m_evLoop->delEvent(m_fd,EPOLLIN);
    }else
    {
        std::cout << "handleRead" << std::endl;
        //接受消息  回调函数处理消息
        m_recvCallbackFun(m_recvBuff);
    }
}
void TcpSocket::handleWrite()
{
    int nwrite = 0;
    bzero(m_sendBuff, BUFF_SIZE);
    strcpy(m_sendBuff, m_sendQueue.front().c_str());
    m_sendQueue.pop();
    while(nwrite != strlen(m_sendBuff))
    {
        nwrite += write(m_fd, m_sendBuff + nwrite, strlen(m_sendBuff)-nwrite);
        if(nwrite == -1)
        {
            perror("write error:");
            close(m_fd);
            break;
        }
    }
    m_evLoop->modEvent(m_fd,EPOLLIN);
}
