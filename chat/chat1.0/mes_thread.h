#ifndef MES_THREAD_H
#define MES_THREAD_H
#include <pthread.h>
#include "global.h"

//void *callback_fun(void);

typedef struct mes_thread
{
    int is_start;
    pthread_t pid;
    pfun callback_fun;
}mes_thread;

void mes_thread_init(mes_thread *thread, pfun callback_fun);
void thread_start(mes_thread *thread);
void thread_run(mes_thread *thread);
#endif // MES_THREAD_H
