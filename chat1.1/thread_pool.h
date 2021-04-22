#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <sys/types.h>
#include <pthread.h>


typedef struct worker
{
    /*回调函数，任务运行时会调用此函数，注意也可声明成其它形式*/
    void *(*process)(void *arg);
    void *arg;/*回调函数的参数*/
    struct worker *next;

}Worker;
/*线程池结构*/
typedef struct
{
    pthread_mutex_t queue_lock;
    pthread_cond_t queue_ready;

    /*链表结构，线程池中所有等待任务*/
    Worker *queue_head;

    /*是否销毁线程池*/
    int shutdown;

    //线程id数组
    pthread_t *threadid;

    /*线程池中允许的活动线程数目*/
    int max_thread_num;
    /*当前等待队列的任务数目*/
    int cur_queue_size;
}ThreadPool;

static ThreadPool *thread_pool = NULL;

/*pool_init*/
//thread_pool: 线程池指针,在函数申请空间
//max_thread_num: 最大线程数
void thread_pool_init(int max_thread_num);
int thread_pool_add_worker(void *(*process)(void *arg), void *arg);
void *thread_routine();
int thread_pool_destroy();
Worker *mk_worker(void *(*process)(void *arg), void *arg);
#endif // THREAD_POOL_H
