#ifndef MSGSERVER_H
#define MSGSERVER_H

#include "tcpserver.h"
#include "tcpsocket.h"

class MsgServer
{
public:
    MsgServer(EventLoop *loop, int port, ThreadPool *pool);
    ~MsgServer();
    void startListen();

private:
    TcpServer *m_tcpServer;
    void handleNewConnection(TcpSocket *socket);
};

#endif // MSGSERVER_H
