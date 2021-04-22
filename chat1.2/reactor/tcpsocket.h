#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "handler.h"
#include "eventloop.h"
#include <queue>
#include <functional>
#include "threadpool.h"

#define BUFF_SIZE 1024
typedef std::function<void (char *)> recvCallback;

class TcpSocket : public Handler
{
public:
    TcpSocket(EventLoop *loop, int fd, ThreadPool *pool);
    ~TcpSocket();
    void setRecvDataHandle(recvCallback callbackFun);
    void sendData(const char *msg);
    ThreadPool *m_pool; //供后边的消息处理函数调用
protected:
    EventLoop *m_evLoop;
    std::queue<std::string> m_sendQueue;
    //virtual void recvData(char *msg);

private:
    char m_recvBuff[BUFF_SIZE];
    char m_sendBuff[BUFF_SIZE];

    recvCallback m_recvCallbackFun;
    void handleRead();
    void handleWrite();
};

typedef TcpSocket* TcpSocketPtr;
typedef std::map<int, TcpSocketPtr> TCPSocketMap; //fd, TCPSocket*

#endif // TCPSOCKET_H
