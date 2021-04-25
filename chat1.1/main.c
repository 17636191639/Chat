/******************************************************************
**   Author:        WXB
**   Create Date:   2021-04-13
**   describe:      4.13 改用epoll处理socket事件
**   versions:      1.1
******************************************************************/
#include <stdio.h>
#include "server.h"
#include "global.h"
#include <pthread.h>
#include <mysql/mysql.h>
#include "sql.h"
#include "thread_pool.h"

#define SERV_PORT 8000

int main()
{
    global_init();
    csql_init(&my_sql, "localhost", "root", "123456", "talkdb",0,NULL,0, 7);

    //消息处理线程
    pthread_t pid;
    int res = pthread_create(&pid, NULL, (void*)handle_message, NULL);
    if(res != 0) {
        printf("Create pthread error!\n");
        return -1;
    }

    epoll_server_init(&g_server, SERV_PORT);  //epoll server初始化

    pthread_t pid2;
    res = pthread_create(&pid2, NULL, (void*)hand_events, NULL);  //消息接受线程
    if(res != 0) {
        printf("Create pthread error!\n");
        return -1;
    }

    /*等待线程结束*/
    pthread_join(pid, NULL);
    pthread_join(pid2, NULL);

    csql_close(&my_sql);
    global_deinit();
    close_ep_server(&g_server);
    return 0;
}
