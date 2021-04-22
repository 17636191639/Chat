#ifndef MSGSOCKET_H
#define MSGSOCKET_H

#include "tcpsocket.h"
typedef void *(*cFun)(void *);
class MsgSocket
{
public:
    MsgSocket(TcpSocket *socket);
    void recvMsg(char *msg);

    void handleMsg();

private:
    std::queue<std::string> m_msgQueue;
    TcpSocket *m_tcpSocket;

    void parse_command(char a[]);
    void parse_login(char a[]);
    void parse_get_ol_users(char a[]);
    void parse_send_msg(char *a);
    void parse_logout(char *id);
    void parse_register(char *msg);
};

#endif // MSGSOCKET_H
