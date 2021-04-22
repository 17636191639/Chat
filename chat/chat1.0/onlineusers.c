#include "onlineusers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

void ollist_init(ollist *list)
{
    list->head = (struct loginfo *)malloc(sizeof(struct loginfo));
    if(list->head == NULL)
    {
        printf("malloc failed\n");
        exit(1);
    }
    list->head->fd = 0;
    //list->head->id = 0;
    list->head->next = NULL;
}
void insert_ollist(ollist *list, int fd, char *id)
{
    struct loginfo *p = mk_loginfo(fd, id);
    p->next = list->head->next;
    list->head->next = p;
}
//void insert_ollist(ollist *list, struct loginfo *p)
//{
//    p->next = list->head->next;
//    list->head->next = p;
//}
struct loginfo *search_loginfo(ollist *list, char *id)
{

    struct loginfo *p = list->head->next;
    while(p != NULL)
    {
        printf("list item id: %s input id:%s\n", p->id, id);
        if(strcmp(p->id, id) == 0)
        {
            return p;
        }

        p = p->next;
    }
    return NULL;
}
int rm_loginfo(ollist *list, char *id) ///一般通过id删除结点   成功返回1
{
    struct loginfo *pre = list->head;
    struct loginfo *p = list->head->next;
    while(p != NULL)
    {
        if(strcmp(p->id, id) == 0)
        {
            pre->next = p->next;
            p->next = NULL;
            free(p);
            return 1;
        }
        pre = p;
        p = p->next;
    }
    return 0;
}
struct loginfo *mk_loginfo(int fd, char *id)
{
    struct loginfo *p = (struct loginfo *)malloc(sizeof(struct loginfo));
    if(p == NULL)
    {
        printf("malloc failed\n");
        exit(1);
    }
    p->fd = fd;
    strcpy(p->id, id);
    //memcpy(p->id, id, strlen(id));//会缺少 \0
    p->next = NULL;

    return p;
}
int ollist_is_empty(ollist *list)
{
    if(list->head->next == NULL)
    {
        return 1;
    }
    return 0;
}
void ollist_deinit(ollist *list)
{
    while(list->head->next != NULL)
    {
        struct loginfo *tmp = list->head->next;
        list->head->next = tmp->next;
        free(tmp);
    }
    free(list->head);
}
void send_msg(char *id, char *msg)
{
    struct loginfo *info = search_loginfo(&g_ollist, id);
    printf("search fd: %d\n", info->fd);
    Write(info->fd, msg, strlen(msg));
}

