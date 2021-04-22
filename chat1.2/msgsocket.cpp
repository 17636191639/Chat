#include "msgsocket.h"
#include <functional>
#include "threadpool.h"
#include <iostream>
#include <string.h>
#include "global.h"
#include "sql.h"



MsgSocket::MsgSocket(TcpSocket *socket)
{
    m_tcpSocket = socket;
    m_tcpSocket->setRecvDataHandle(std::bind(&MsgSocket::recvMsg, this, std::placeholders::_1));
}

void MsgSocket::recvMsg(char *msg)
{
    //将消息加入线程池进行处理

    //1. 先将消息加入消息队列
    m_msgQueue.push(msg);
    std::cout << "m_msgQueue.push(msg);" << msg << std::endl;
    //2. 给线程池加入任务
    if(m_tcpSocket->m_pool != nullptr)
    {
        Task *task = new Task;
        task->function = std::bind(&MsgSocket::handleMsg, this);
        m_tcpSocket->m_pool->addTask(task);
    }else
    {
        std::cout << "thread pool is NULL" << std::endl;
    }

}

void MsgSocket::handleMsg()
{
    std::string msg = m_msgQueue.front();
    m_msgQueue.pop();
    char *cmd = &msg[0];
    std::cout << "parse_command: " << cmd << std::endl;
    parse_command(cmd);
    //m_tcpSocket->sendData(msg.c_str());
}

void MsgSocket::parse_command(char a[])
{

    switch (a[0])
    {
        //L#  id|pswd
        case cmd_login: parse_login(&a[2]); break;
        //O#  id
        case cmd_get_ol_users:parse_get_ol_users(&a[2]);break;
        //M#  id|otherID|content
        case cmd_send_msg:parse_send_msg(&a[2]);break;
        //E#  id
        case cmd_logout:parse_logout(&a[2]);break;
        //R#  id|passwd
        case cmd_register:parse_register(&a[2]);break;
        default:
            break;
    }
}
void MsgSocket::parse_login(char a[])
{
    //解析id pswd
    char id[8] = {0};
    int d = 0;
    char pswd[8] = {0};
    int p = 0;

    int id_flag = 1;

    int i = 0;

    int len = strlen(a);
    for(i = 0; i < len; i++)
    {

        if(a[i] == '|')
        {
            id_flag = 0;
            continue;
        }

        if(id_flag == 1)
        {
            id[d++] = a[i];

        }else
        {
            pswd[p++] = a[i];

        }
    }
    id[d] = '\0';
    pswd[p] = '\0';
    printf("id: %s, paswd: %s\n", id, pswd);
    userinfo_clear(&g_userinfo);

    //判断是否已经登录
    struct loginfo *info = search_loginfo(&g_ollist, id);
    if(info != NULL)
    {
        printf("该用户已登录\n");
        char send_msg[100] = {0};
        sprintf(send_msg, "L#?|%s|already login!", id);
        //Write(fd, send_msg, strlen(send_msg));
        //do_write(&g_server, fd, send_msg);
        m_tcpSocket->sendData(send_msg);
        return ;
    }
    ///通过数据库查找信息将信息写到g_userinfo,比对信息
    if(select_userinfo_by_id(&my_sql, id))
    {

        if((strcmp(g_userinfo.id, id) == 0) && (strcmp(g_userinfo.pswd, pswd) == 0))
        {
            char send_msg[100] = {0};
            sprintf(send_msg, "L#!|%s|%s|%s", g_userinfo.id, g_userinfo.pswd, g_userinfo.name);

            insert_ollist(&g_ollist, m_tcpSocket, g_userinfo.id);  ///登录成功 将信息存到登录链表里


            //Write(fd, send_msg, strlen(send_msg));
            //do_write(&g_server, fd, send_msg);
            m_tcpSocket->sendData(send_msg);
        }else
        {
            printf("账号或密码不正确");
        }
    }

}
void MsgSocket::parse_get_ol_users(char id[])
{

    printf("from id get ol users request : %s\n",id);

    char *str = "O#!|%s";
    char send[300] = {0};
    sprintf(send, str, id);

    //int len = strlen(g_userinfo.id);


    struct loginfo *p = g_ollist.head->next;
    while(p != NULL)
    {
        strcat(send, "|");
        strcat(send, p->id);
        p = p->next;
    }
    printf("send msg %s\n", send);
    //回应消息 O#!|id|Userid|send...
    //send_msg(id, send);
    m_tcpSocket->sendData(send);

}
void MsgSocket::parse_send_msg(char *a)
{
    printf("parse command : %s", a);
    // id|otherID|content
    char from_id[8] = {0};
    char other_id[8] = {0};
    char msg[100] = {0};

    int id_i = 0;
    int o_id_i = 0;
    int msg_i = 0;
    int type_flag = 0;

    while(*a != '\0')
    {
        //printf("cur char :%c, type flag: %d\n", *a, type_flag);
        if(*a == '|')
        {
            type_flag++;
            a++;
            continue;
        }
        if(type_flag == 0)
        {
            //id
            printf("%c\n", *a);
            from_id[id_i++] = *a;
        }
        if(type_flag == 1)
        {
            //other id
            other_id[o_id_i++] = *a;
        }
        if(type_flag == 2)
        {
            //msg
            msg[msg_i++] = *a;
        }
        a++;
    }
    //根据otherid找到要转发的fd将消息发送
    //发送格式: M#&|id|fromid|msg   //M#&|id|A|msg

    //发送所有人
    if(strcmp(other_id, "A") == 0)
    {
        struct loginfo *p = g_ollist.head->next;
        while(p != NULL)
        {
            //合成发送消息
            char msg_buf[100] = {0};


            sprintf(msg_buf, "M#&|%s|%s|%s", p->id, from_id, msg);

            p->m_sock->sendData(msg_buf);
            p = p->next;
        }
        return ;
    }
    //发送到相应ID
    struct loginfo *info = search_loginfo(&g_ollist, other_id);
    if(info != NULL)
    {
        char msg_buf[100] = {0};

        //M#&|002|xxx|0001231
        sprintf(msg_buf, "M#&|%s|%s|%s", other_id, from_id, msg);
        printf("send relay msg: %s from id: %s\n",msg_buf, from_id);

        //m_tcpSocket->sendData(msg_buf);
        info->m_sock->sendData(msg_buf);
    }else
    {
        printf("msg relay failed\n");
    }
}
void MsgSocket::parse_logout(char *id)
{
    //从在线链表里删除登录信息
    if(rm_loginfo(&g_ollist, id))
    {
        printf("id: %s logout!\n", id);
    }
}
void MsgSocket::parse_register(char *msg)
{
    // R#  id|pswd
    char id[8] = {0};
    int d = 0;
    char pswd[8] = {0};
    int p = 0;
    int is_paswd = 0;
   //解析id|paswd
    while(*msg != '\0')
    {
        if(*msg == '|')
        {
            is_paswd = 1;
            msg++;
            continue;
        }
        if(is_paswd)
        {
            pswd[p++] = *msg;
        }else
        {
            id[d++] = *msg;
        }
        msg++;
    }
    //判断是否已注册

    if(!select_userinfo_by_id(&my_sql, id))  //返回0说明没有注册
    {
        if(insert_userinfo(&my_sql, id, pswd, "default"))
        {
            char send_msg[20] = {0};
            sprintf(send_msg, "R#!|id",id);
            printf("注册成功!");
            m_tcpSocket->sendData(send_msg);

        }
    }
}
