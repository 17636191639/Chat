/******************************************************************
**   Author:        WXB
**   Create Date:   2021-04-15
**   描   述:        1.1   4.13 改用epoll处理socket事件
**                  1.2   4.15 将工程按模块分类
**                              使用Reactor模型将事件分类, 使用线程池处理消息
**   版   本:        1.2
******************************************************************/


#include "msgserver.h"
#include "global.h"
#define SERV_PORT 8000

int main()
{
    global_init();
    //数据库初始化
    csql_init(&my_sql, "localhost", "root", "123456", "talkdb",0,NULL,0, 7);

    EventLoop loop;
    ThreadPool pool(4);

    MsgServer *tcpServer = new MsgServer(&loop, SERV_PORT, &pool);
    tcpServer->startListen();

    loop.runLoop();

    //关闭数据库
    csql_close(&my_sql);
    //全局数据释放
    global_deinit();

    return 0;
}
