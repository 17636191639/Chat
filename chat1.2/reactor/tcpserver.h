#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "handler.h"
#include "tcpsocket.h"
//#include <functional>

typedef std::function<void (TcpSocket *)> newConnectCallback;
//typedef void (* pfun)(TcpSocket *);

class TcpServer : public Handler
{
public:
    TcpServer(EventLoop *loop, int port, ThreadPool *pool);
    ~TcpServer();

    int startListen(void);
    void setNewTcpConnectionCallback(newConnectCallback callbackFun);
    ThreadPool *m_pool;
protected:
    int m_port;
    TCPSocketMap m_socketMap;



private:

    EventLoop *m_evLoop;
    newConnectCallback m_connectCallback;
    void handleAccept();
};

#endif // TCPSERVER_H
