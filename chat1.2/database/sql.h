#ifndef SQL_H
#define SQL_H

#include <mysql/mysql.h>

struct CSQL
{
    MYSQL *conn_ptr;  //MYSQL结构体指针
    unsigned int timeout;//超时时间
    char *host;  //主机名/ip
    char *username;  //用户名
    char *passwd;  //密码
    char *dbname; //数据库名
    int port;  //MySQL服务器的TCP/IP端口
    char *unix_socket;//连接类型
    unsigned int client_flag;//运行ODBC数据库的标记
};
void csql_init(struct CSQL *p, char *host, char *username, char *passwd, char *dbname, int port,
               char *unix_socket, unsigned int client_flag, unsigned int timeout);
void set_connect_timeout(struct CSQL *p);
void real_connect(struct CSQL *p);
void exec_sql(struct CSQL *p, char *sql);
void csql_close(struct CSQL *p);

int select_userinfo_by_id(struct CSQL *p, const char *id);  //返回1执行成功
int insert_userinfo(struct CSQL *p, const char *id, const char *pswd, const char *name);//返回1执行成功
#endif // SQL_H
