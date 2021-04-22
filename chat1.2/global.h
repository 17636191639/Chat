#ifndef GLOBAL_H
#define GLOBAL_H

#include "link_queue.h"
#include "userinfo.h"
#include "sql.h"
#include "onlineusers.h"

#define MAXBUF 1024   //接受消息数组大小

typedef void (*pfun)(void);
//typedef void (*send_to_client)(epoll_server *, int, char *) ;   //给客户端发送消息函数
//send_to_client write_to_client = do_write;


enum
{
    cmd_login = 'L',
    cmd_get_ol_users = 'O',
    cmd_send_msg = 'M',
    cmd_logout = 'E',
    cmd_register = 'R'

};
extern struct CSQL my_sql;  //数据库
extern mes_queue g_queue;   //消息队列
extern ollist g_ollist;     //记录登录用户网络描述符信息

extern struct userinfo g_userinfo;

void global_init(void);
void global_deinit(void);

#endif // GLOBAL_H
