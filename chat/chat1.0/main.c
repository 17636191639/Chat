#include <stdio.h>
#include "server.h"
#include "global.h"
#include <pthread.h>
#include <mysql/mysql.h>
#include "sql.h"

#define SERV_PORT 8000



int main()
{
    global_init();


    csql_init(&my_sql, "localhost", "root", "123456", "talkdb",0,NULL,0, 7);

    pthread_t pid;

    struct select_server myserver;

    int res = pthread_create(&pid, NULL, (void*)handle_message, NULL);  //创建线程
    if(res != 0) {
        printf("Create pthread error!\n");
        return -1;
    }

    select_server_init(&myserver, SERV_PORT);  //创建select服务器
    select_start(&myserver);  //处理消息
    /*等待线程结束*/
    pthread_join(pid, NULL);
////////////////////////////////////////////////
    csql_close(&my_sql);
    global_init();
    return 0;
}
