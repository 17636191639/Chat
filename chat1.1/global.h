#ifndef GLOBAL_H
#define GLOBAL_H

#include "link_queue.h"
#include "wrap.h"
#include "userinfo.h"
#include "sql.h"
#include "onlineusers.h"
#include "server.h"

#define MAXBUF 1024   //接受消息数组大小

typedef void (*pfun)(void);
//typedef void (*send_to_client)(epoll_server *, int, char *) ;   //给客户端发送消息函数
//send_to_client write_to_client = do_write;


enum
{
    cmd_login = 'L',
    cmd_get_ol_users = 'O',
    cmd_send_msg = 'M',
    cmd_logout = 'E'
};
struct CSQL my_sql;  //数据库
mes_queue g_queue;   //消息队列
ollist g_ollist;     //记录登录用户网络描述符信息

struct userinfo g_userinfo;
epoll_server g_server; //服务器

void global_init(void);
void global_deinit(void);

void handle_message();
void hand_events();

void parse_command(char a[], int len, int fd);
void parse_login(char a[], int len, int fd);
void parse_get_ol_users(char a[], int len);
void parse_send_msg(char *a);
void parse_logout(char *id);
#endif // GLOBAL_H
