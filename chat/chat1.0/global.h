#ifndef GLOBAL_H
#define GLOBAL_H

#include "link_queue.h"
#include "wrap.h"
#include "userinfo.h"
#include "sql.h"
#include "onlineusers.h"

#define MAXBUF 100   //接受消息数组大小
#define MAXWAIT 20   //同时等待连接数大小
typedef void (*pfun)(void);
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


void global_init(void);
void global_deinit(void);
void handle_message();
void parse_command(char a[], int len, int fd);
void parse_login(char a[], int len, int fd);
void parse_get_ol_users(char a[], int len);
void parse_send_msg(char *a);
void parse_logout(char *id);
#endif // GLOBAL_H
