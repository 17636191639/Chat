#ifndef ONLINEUSERS_H
#define ONLINEUSERS_H

struct loginfo
{
    char id[8];//用户id   //根据用户id找到相应的网络文件描述符
    int fd; //网络文件描述符
    struct loginfo *next;
};
typedef struct ollist
{
    struct loginfo *head;
}ollist;

void ollist_init(ollist *list);
void insert_ollist(ollist *list, int fd, char *id);
//void insert_ollist(ollist *list, struct loginfo *p);
struct loginfo *search_loginfo(ollist *list, char *id);
int rm_loginfo(ollist *list, char *id); ///一般通过id删除结点   成功返回1
int ollist_is_empty(ollist *list);
struct loginfo *mk_loginfo(int fd, char *id);

void ollist_deinit(ollist *list);
void send_msg(char *id, char *msg);
//////////////////////////////////  Test case  ///////////////////////////
//char buf[10];

//int i = 0;
//for(i = 0; i < 10; i++)
//{
//    sprintf(buf, "12345%d", i);
//    insert_ollist(&g_ollist, i, buf);
//}
//struct loginfo *p = search_loginfo(&g_ollist, "123458");
//if(p != NULL)
//{
//    printf("find fd: %d\n", p->fd);
//}
/////////////////////////////////  end   /////////////////////////////
#endif // ONLINEUSERS_H
