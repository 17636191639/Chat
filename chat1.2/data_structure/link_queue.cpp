#include "link_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void datatype_init(DATATYPE *p)
{
    memset(p->mes,0,MAXBUF);
}


void queue_init(mes_queue *p)
{
    DATATYPE a = {0};

    //p->sentry = {a, nullptr, nullptr};
    p->sentry = (node *)malloc(sizeof(node));
    if(p->sentry == NULL)
    {
        printf("malloc failed\n");
        exit(-1);
    }
    p->sentry->data = a;
    p->sentry->next = NULL;

    p->front = p->sentry;
    p->rear = p->sentry;
}
void en_queue(mes_queue *p, DATATYPE item) /////队尾插入   rear一直指向队尾
{
    node *n = mk_node(item);

    p->rear->next = n;
    p->rear = n;
}
node *mk_node(DATATYPE item)
{
    node *p = (node *)malloc(sizeof(node));
    if (p == NULL)
    {
        printf("malloc failed\n");
        exit(1);
    }

    p->data = item;
    p->next = NULL;

    return p;
}
DATATYPE de_queue(mes_queue *p)
{
    node *n = p->front->next;
    DATATYPE tmp = n->data;
    p->front->next = n->next;

    free(n);
    return tmp;

}

int is_empty(mes_queue *p)
{
    if(p->front->next == NULL)
    {
        p->rear = p->front;  //当队列放空的时候，需要让尾指针也指到头结点
        return 1;
    }
    return 0;
}
void queue_deinit(mes_queue *p)
{

    while(!is_empty(p))
    {
        de_queue(p);
    }
    free(p->sentry); //将头结点释放

}
