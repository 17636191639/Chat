#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <queue>
#include <list>
#include <string>
#include <functional>

#include "thread.h"

///
/// \brief 存放任务信息的结构体
/// \details 使用C++11 std::function()
typedef struct
{
   std::function<void()> function;
//   void *arg;
}Task;


///
/// \brief 由于处理线程池内线程同步的特殊类
/// \details 通用的类具体见[linux高性能服务器编程]第14章
class Locker
{
public:
    Locker()
    {
        ::pthread_mutex_init(&m_mutex, nullptr);
        ::pthread_cond_init(&m_condition, nullptr);
    }
    ~Locker()
    {
        ::pthread_mutex_destroy(&m_mutex);
        ::pthread_cond_destroy(&m_condition);
    }

    void lock(void){ ::pthread_mutex_lock(&m_mutex);}
    void unlock(void){ ::pthread_mutex_unlock(&m_mutex);}
    void wait(){ ::pthread_cond_wait(&m_condition, &m_mutex);}

    void wakeOne(void){ ::pthread_cond_signal(&m_condition);}
    void wakeAll(void){ ::pthread_cond_broadcast(&m_condition);}

private:
    pthread_mutex_t m_mutex;//互斥锁
    pthread_cond_t m_condition;//条件变量
};

///
/// \brief 简易的线程池类
/// \details 实现了线程池通用的逻辑
class ThreadPool
{
public:
    ThreadPool(int threadNum);
    ~ThreadPool();
    int addTask(Task* task);//将任务添加进线程
    int getTaskSize();//获取任务队列中任务的剩余数量
    int stopAll();//退出所有线程

protected:
    void taskProcFunc(void);//线程的回调函数

private:
    int m_threadNum;//线程池的初始化大小
    bool m_isExit;//线程退出标志
    std::queue<Task*> m_taskQueue;    //任务队列
    ThreadList m_threadList;   //线程链表
    Locker m_locker;
};
#endif //_THREAD_POOL_H
