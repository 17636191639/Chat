#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void thread_pool_init(int max_thread_num)//最大线程数
{
    thread_pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    if(thread_pool == NULL)
    {
        printf("pthread malloc failed !\n");
        exit(1);
    }
    //初始化锁跟条件变量
    pthread_mutex_init(&(thread_pool->queue_lock), NULL);
    pthread_cond_init(&(thread_pool->queue_ready), NULL);

    thread_pool->queue_head = NULL; //工作队列

    thread_pool->max_thread_num = max_thread_num;
    thread_pool->cur_queue_size = 0;//工作队列中的任务数

    thread_pool->shutdown = 0;

    thread_pool->threadid = (pthread_t *)malloc(max_thread_num *sizeof(pthread_t));
    int i = 0;
    for(i = 0; i < max_thread_num; i++)   //创建多个线程
    {
        pthread_create(&(thread_pool->threadid[i]), NULL, thread_routine, NULL);
    }
}
int thread_pool_add_worker(void *(*process)(void *arg), void *arg)
{
    //创建新任务
    Worker *new_worker = mk_worker(process, arg);

    pthread_mutex_lock(&(thread_pool->queue_lock));//上锁防止队列在修改过程中发生改变

    /*将任务加入到等待队列中*/
    Worker *member = thread_pool->queue_head;
    if(member != NULL)
    {
        while(member->next != NULL)
        {
            member = member->next;
        }
        member->next = new_worker;
    }else
    {
        thread_pool->queue_head = new_worker;
    }

    assert(thread_pool->queue_head != NULL);

    thread_pool->cur_queue_size++;
    pthread_mutex_unlock(&(thread_pool->queue_lock));//解锁
    /*好了，等待队列中有任务了，唤醒一个等待线程；
    注意如果所有线程都在忙碌，这句没有任何作用*/
    pthread_cond_signal(&(thread_pool->queue_ready));
    return 0;
}

//线程调用的回调函数  处理任务队列里的任务
void *thread_routine()
{
    printf("starting thread 0x%x\n", pthread_self());
    while(1)
    {
        pthread_mutex_lock(&(thread_pool->queue_lock));
        /*如果等待队列为0并且不销毁线程池，则处于阻塞状态; 注意
        pthread_cond_wait是一个原子操作,等待条件变量的成立，等待前会解锁，唤醒后会加锁*/
        while(thread_pool->cur_queue_size == 0 && !thread_pool->shutdown)
        {
            printf("thread 0x%x is waiting\n", pthread_self());
            pthread_cond_wait(&(thread_pool->queue_ready), &(thread_pool->queue_lock));
        }

        /*线程池要销毁了*/
        if(thread_pool->shutdown)
        {
            /*遇到break,continue,return等跳转语句，千万不要忘记先解锁*/
            pthread_mutex_unlock(&(thread_pool->queue_lock));
            printf("thread 0x%x will exit\n", pthread_self());
            pthread_exit(NULL);
        }

        printf("thread 0x%x is starting to work\n", pthread_self());

        /*assert是调试的好帮手*/
        assert(thread_pool->cur_queue_size != 0);
        assert(thread_pool->queue_head != NULL);

        /*等待队列长度减去1，并 取出链表中的头元素*/
        thread_pool->cur_queue_size--;
        Worker *worker = thread_pool->queue_head;
        thread_pool->queue_head = worker->next;

        pthread_mutex_unlock(&(thread_pool->queue_lock));

        /*调用回调函数，执行任务*/
        (*(worker->process))(worker->arg);

        free(worker);
        worker = NULL;
    }
    /*这一句应该是不可达的*/
    pthread_exit(NULL);
}


int thread_pool_destroy()
{
    if(thread_pool->shutdown)
    {
        return -1;/*防止两次调用*/
    }

    thread_pool->shutdown = 1;

    /*唤醒所有等待线程，线程池要销毁了*/
    pthread_cond_broadcast(&(thread_pool->queue_ready));

    /*阻塞等待线程退出，否则就成僵尸了*/
    int i;
    for(i = 0; i < thread_pool->max_thread_num; i++)
    {
        pthread_join(thread_pool->threadid[i], NULL);
    }

    free(thread_pool->threadid);

    /*销毁等待队列*/
    ThreadPool *head = NULL;
    while(thread_pool->queue_head != NULL)
    {
        head = thread_pool->queue_head;
        thread_pool->queue_head = thread_pool->queue_head->next;
        free(head);
    }
    /*条件变量和互斥量也别忘了销毁*/
    pthread_mutex_destroy(&(thread_pool->queue_lock));
    pthread_cond_destroy(&(thread_pool->queue_ready));

    free(thread_pool);
    /*销毁后指针置空是个好习惯*/
    thread_pool=NULL;
    return 0;
}

Worker *mk_worker(void *(*process)(void *arg), void *arg)
{
     Worker *new_worker = (Worker *) malloc(sizeof(Worker));
     if(new_worker == NULL)
     {
         perror("malloc");
         exit(1);
     }
     new_worker->process = process;
     new_worker->arg = arg;
     new_worker->next = NULL;/*别忘置空*/

     return new_worker;
}
