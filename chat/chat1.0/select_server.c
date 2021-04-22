#include "server.h"
#include "wrap.h"
#include "global.h"
#include <string.h>

void init_signal(void)//设置信号处理,SIG_IGN表示忽略信号,SIG_DFL表示使用信号的默认处理方式
{
    signal(SIGCHLD, SIG_DFL); //SIGCHLD信号 在一个进程终止时会产生该信号将该信号发送给父进程  系统默认忽略
    signal(SIGPIPE, SIG_IGN);  //当一个进程向某个已收到RST的套接字执行写操作时，内核向该进程发送SIGPIPE信号
                                //为了防止客户端进程终止，而导致服务器进程被SIGPIPE信号终止，因此服务器程序要处理SIGPIPE信号
}


void select_server_init(struct select_server *p, int port)
{
    p->port = port;
    init_signal();
    p->listenfd = socket_bind(p->port);
    start_listen(p->listenfd, MAXWAIT);

    fdset_init(p);
}
void select_start(struct select_server *p)
{

    while(1)
    {

        p->nready = my_select(p);
        if(is_new_connnect(p))
        {
            add_new_connect(p);
            if(--p->nready == 0)
            {
                continue;
            }
        }
        check_client(p);
    }
}
//创建套接字并绑定
//返回监听的文件描述符d
int socket_bind(int port)
{
    int listenfd;
    struct sockaddr_in servaddr; //在函数调用中需强制转换为sockaddr
    listenfd = socket(AF_INET,SOCK_STREAM,0);//ipv 4 流式输出    //创建套接字
    if (listenfd < 0)
    {
        perror("socket error:");
        exit(1);
    }
    //一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。
    int reuse_addr = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) == -1)
    {
        perror("setsockopt error:");
        return -1;
    }

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET; //ipv4  地址族
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定所有网卡所有IP
    servaddr.sin_port = htons(port); //端口
    if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)  //绑定端口号
    {
        perror("bind error: ");
        exit(1);
    }
    printf("listen on: %d,listenfd=%d\n",port,listenfd);
    return listenfd;

}
//start listening
void start_listen(int listenfd, int backlog)
{
    if(listen(listenfd, backlog) < 0)
    {
        perror("listen failed:");
        exit(1);
    }
    printf("listen start \n");
}

//初始化客户端数组
void fdset_init(struct select_server *p)
{
    p->maxfd = p->listenfd;
    p->maxi = -1;
    int i = 0;
    for(i = 0; i < FD_SETSIZE; i++)
    {
        p->client[i] = -1;
    }
    FD_ZERO(&(p->allset));
    FD_SET(p->listenfd, &(p->allset));//将listenfd放入set
}

int my_select(struct select_server *p)
{
    p->rset = p->allset;	/* structure assignment */
    int n = select(p->maxfd+1, &(p->rset), NULL, NULL, NULL); //记录状态变化的文件描述符个数
    if (n < 0)
    {
        perror("select error ");
        exit(1);
    }
    return n;
}

 int is_new_connnect(struct select_server *p)
 {
     return FD_ISSET(p->listenfd, &(p->rset));
 }
 //返回新连接的文件描述符
 int new_connectfd(int listenfd)
 {
     struct sockaddr_in	cliaddr;
     int connectfd = 0;//新连接的文件描述符
     socklen_t cliaddr_len = sizeof(cliaddr);
     char str[INET_ADDRSTRLEN]; //用来保存ip

     connectfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

     printf("received from %s at PORT %d\n",
                inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
                ntohs(cliaddr.sin_port));
     return connectfd;
 }


 void add_new_connect(struct select_server *p)
 {

     int connectfd = new_connectfd(p->listenfd);
     int i;
     for (i = 0; i < FD_SETSIZE; i++)
     {
         if (p->client[i] < 0)
         {
             p->client[i] = connectfd; /* save descriptor *///将文件描述符保存到客户端数组以便下次发数据时查找
             break;
         }
     }
     if (i == FD_SETSIZE) {
         fputs("too many clients\n", stderr);
         exit(1);
     }

     //将新的文件描述符放到描述词组里
     FD_SET(connectfd, &(p->allset));	/* add new descriptor to set */
     if (connectfd > p->maxfd)
     {
         p->maxfd = connectfd; /* for select */
     }
     if (i > p->maxi)
     {
         p->maxi = i;	/* max index in client[] array */
     }

 }
 //遍历所有客户端检查其文件描述符是否是置位状态，如果是则有新消息
 void check_client(struct select_server *p)
 {
     int sockfd;
     char buf[MAXBUF] = {0};
     int nbytes = 0;
     int i;
     for (i = 0; i <= p->maxi; i++)
     {	/* check all clients for data */
         if ( (sockfd = p->client[i]) < 0)
         {
             continue;
         }

         if (FD_ISSET(sockfd, &(p->rset)))
         {
             if ( (nbytes = Read(sockfd, buf, MAXBUF)) == 0)
             {
                 //如果是空消息则关闭文件描述符
                 /* connection closed by client */
                 Close(sockfd);
                 FD_CLR(sockfd, &(p->allset));
                 p->client[i] = -1;
             } else
             {
                 //消息处理
                 //Write(sockfd, buf, nbytes);//将消息从网络写回
                 //把消息加到消息队列

                 DATATYPE mes;
                 mes.fd = sockfd;
                 mes.nbytes = nbytes;
                 bzero(mes.mes, MAXBUF);

                 //memcpy(mes.mes, buf, nbytes);
                 strcpy(mes.mes, buf);
                 printf("new message enqueue: %s data size:%d\n", mes.mes, nbytes);
                 en_queue(&g_queue, mes);
             }

             if (--(p->nready) == 0)
                 break;	/* no more readable descriptors */
         }
     }
 }




