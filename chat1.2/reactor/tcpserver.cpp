#include "tcpserver.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <functional>

TcpServer::TcpServer(EventLoop *loop, int port, ThreadPool *pool)
{
    struct sockaddr_in servaddr;
    m_fd = socket(AF_INET,SOCK_STREAM,0);
    if(m_fd == -1)
    {
        perror("socket error:");
        exit(1);
    }
    m_pool = pool;

    //一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。
    int reuse_addr = 1;
    if(setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) == -1)
    {
        exit(1);
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //inet_pton(AF_INET,ip,&servaddr.sin_addr);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定所有网卡所有IP
    servaddr.sin_port = htons(port);
    if(bind(m_fd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
    {
        perror("bind error: ");
        exit(1);
    }

    m_port = port;
    m_evLoop = loop;
    m_evLoop->registerServerEvent(m_fd,this);
}
TcpServer::~TcpServer()
{
    close(m_fd);
}

int TcpServer::startListen(void)
{
    listen(m_fd,LISTENQ);
    printf("listen on: %d,listenfd=%d\n", m_port, m_fd);
}

void TcpServer::setNewTcpConnectionCallback(newConnectCallback callbackFun)
{
    m_connectCallback = std::move(callbackFun);
}


void TcpServer::handleAccept()
{
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen = sizeof(cliaddr);
    int fd = accept(m_fd, (struct sockaddr*)&cliaddr, &cliaddrlen);
    if(fd == -1)
    {
        perror("accpet error:");
    }else
    {
        printf("accept a new client: %s:%d,fd=%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port,fd);

        //添加一个客户描述符和事件    读写事件处理
        TcpSocket *socket = new TcpSocket(m_evLoop, fd, m_pool);    //socket继承handler
        m_socketMap[fd] = socket;
        if(m_evLoop->registerSocketEvent(fd, socket))
        {
            m_connectCallback(socket);  //执行新链接 处理函数
        }
    }
}
