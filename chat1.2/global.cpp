#include "global.h"
struct CSQL my_sql;  //数据库
mes_queue g_queue;   //消息队列
ollist g_ollist;     //记录登录用户网络描述符信息

struct userinfo g_userinfo;

void global_init(void)
{
    queue_init(&g_queue);
    ollist_init(&g_ollist);
}
void global_deinit(void)
{
    queue_deinit(&g_queue);
    ollist_deinit(&g_ollist);
}


