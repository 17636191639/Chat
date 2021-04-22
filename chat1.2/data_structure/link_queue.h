#ifndef LINK_QUEUE_H
#define LINK_QUEUE_H

#define MAXBUF 1024

typedef struct message
{
     int fd;
     int nbytes;
     char mes[MAXBUF];
}message;

typedef message DATATYPE;
typedef struct node
{
    DATATYPE data;
    struct node *next;
}node;

typedef struct mes_queue
{
    node *sentry;
    node *front;
    node *rear;
}mes_queue;

void datatype_init(DATATYPE *p);


void queue_init(mes_queue *p);
void en_queue(mes_queue *p, DATATYPE item);
node *mk_node(DATATYPE item);
DATATYPE de_queue(mes_queue *p);
int is_empty(mes_queue *p);
void queue_deinit(mes_queue *p);

///////////////////////////////////////////////////////test case///////////////////////////////////
//queue_init(&g_queue);
//int i = 0;
//for(i = 0; i < 10; i++)
//{
//    DATATYPE insert;
//    insert.fd = 10;
//    char *s = "hello";
//    strcpy(insert.mes, s);
//    en_queue(&g_queue, insert);
//}
//while(!is_empty(&g_queue))
//{
//    DATATYPE tmp = de_queue(&g_queue);
//    printf("de_queue: %d  %s\n", tmp.fd, tmp.mes);
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // LINK_QUEUE_H
