#include "sql.h"

#include <stdio.h>
#include <string.h>
#include "global.h"




void csql_init(struct CSQL *p, char *host, char *username, char *passwd, char *dbname, int port,
               char *unix_socket, unsigned int client_flag, unsigned int timeout)
{
    p->conn_ptr = mysql_init(NULL);//初始化MYSQL结构体
    p->timeout = timeout;
    p->host = host;
    p->username = username;
    p->passwd = passwd;
    p->dbname = dbname;
    p->port = port;
    p->unix_socket = unix_socket;
    p->client_flag = client_flag;

    if(!p->conn_ptr)
    {
        printf("mysql_init failed!\n");
        return ;
    }
    set_connect_timeout(p);
    real_connect(p);
}

void set_connect_timeout(struct CSQL *p)
{
    int ret = 0;
    ret = mysql_options(p->conn_ptr,MYSQL_OPT_CONNECT_TIMEOUT,(const char*)&(p->timeout));//设置超时选项
    if(ret)
    {
        printf("Options Set ERRO!\n");
    }
}

void real_connect(struct CSQL *p)
{
    p->conn_ptr = mysql_real_connect(p->conn_ptr,p->host,p->username,p->passwd,p->dbname,p->port,p->unix_socket,p->client_flag);//连接MySQL
    if(p->conn_ptr)
    {
        printf("Connection Succeed!\n");
    }else
    {
        printf("Connection Failed!\n");
        if(mysql_errno(p->conn_ptr))
        {
            printf("Connect Erro:%d %s\n",mysql_errno(p->conn_ptr),mysql_error(p->conn_ptr));//返回错误代码、错误消息
        }
    }

}
int select_userinfo_by_id(struct CSQL *p, const char *id)
{
    //////////////////////////字符串拼接/////////////////
    char *sql = "select * from user where id='%s';";
    char sql_buf[100] = {0};

    sprintf(sql_buf, sql, id);
    printf("sprintf::: %s\n", sql_buf);
    /////////////////////////////////////////////////////
    int ret = mysql_query(p->conn_ptr, sql_buf); //执行SQL语句
    if(!ret)
    {
        MYSQL_RES *res_ptr;//获取结果标识符
        res_ptr = mysql_use_result(p->conn_ptr);   //mysql_store_result()
        if(res_ptr)
        {
//            int col = mysql_num_fields(res_ptr);
//            printf("number of fields returned: %d\n", col);

            MYSQL_ROW row;
            int i = 0;
            while ((row = mysql_fetch_row(res_ptr)))
            {
                i++;
                memcpy(g_userinfo.id, row[0], strlen(row[0]));
                memcpy(g_userinfo.pswd, row[1], strlen(row[1]));
                memcpy(g_userinfo.name, row[2], strlen(row[2]));
//                printf("id: %s ", (row[0] == NULL) && (!strlen(row[0])) ? "NULL" : row[0]) ;
//                printf(" passwd: %s ", (row[1] == NULL) && (!strlen(row[1])) ? "NULL" : row[1]) ;
//                printf(" name: %s ", (row[2] == NULL) && (!strlen(row[2])) ? "NULL" : row[2]) ;
//                printf("\n");
                printf("add on line user id: %s pswd: %s name %s\n", g_userinfo.id, g_userinfo.pswd, g_userinfo.name);


            }

            mysql_free_result(res_ptr);

            if(i == 0)
            {
                return 0;
            }
            return 1;
        }else
        {
            return 0;
        }
    }else
    {
        printf("Connect Erro:%d %s\n",mysql_errno(p->conn_ptr),mysql_error(p->conn_ptr));//返回错误代码、错误消息
        return 0;
    }

}

void exec_sql(struct CSQL *p, char *sql)
{
    int ret = mysql_query(p->conn_ptr, sql); //执行SQL语句
    if(!ret)
    {
        MYSQL_RES *res_ptr;//获取结果标识符
        res_ptr = mysql_use_result(p->conn_ptr);   //mysql_store_result()
        if(res_ptr)
        {
            int col = mysql_num_fields(res_ptr);
            printf("number of fields returned: %d\n", col);

            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res_ptr)))
            {
                int i = 0;
                for(i = 0; i < col; i++)
                {
                    printf("  %s ", (row[i] == NULL) && (!strlen(row[i])) ? "NULL" : row[i]) ;
                }
                printf("\n");
            }

            mysql_free_result(res_ptr);
        }
    }else
    {
        printf("Connect Erro:%d %s\n",mysql_errno(p->conn_ptr),mysql_error(p->conn_ptr));//返回错误代码、错误消息
        return ;
    }

}

void csql_close(struct CSQL *p)
{
    mysql_close(p->conn_ptr);
    printf("Connection closed!\n");
}


