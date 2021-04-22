#include "msgserver.h"
#include <iostream>
#include <functional>
#include "msgsocket.h"

MsgServer::MsgServer(EventLoop *loop, int port, ThreadPool *pool)
{
    m_tcpServer = new TcpServer(loop, port, pool);
    m_tcpServer->setNewTcpConnectionCallback(std::bind(&MsgServer::handleNewConnection, this, std::placeholders::_1));
}
MsgServer::~MsgServer()
{
    delete m_tcpServer;
}
void MsgServer::startListen()
{
    m_tcpServer->startListen();
}
void MsgServer::handleNewConnection(TcpSocket *socket)
{
    //std::cout << "callback  MsgServer::handleNewConnection(TcpSocket *socket)" << std::endl;
    MsgSocket *msgSock = new MsgSocket(socket);
}
