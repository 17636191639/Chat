#include "threadpool.h"
#include <cstdio>

ThreadPool::ThreadPool(int threadNum):
    m_threadNum(threadNum)
{
    m_isExit = false;
    m_threadNum = threadNum;
    for(int i = 0; i < m_threadNum; i++)
    {
        Thread *p = new Thread(std::bind(&ThreadPool::taskProcFunc,this));
        p->start();
        m_threadList.push_back(p);
    }
    ::printf("I have created %d threads.\n", m_threadNum);
}

ThreadPool::~ThreadPool()
{
    for(ThreadList::iterator it = m_threadList.begin();
                            it != m_threadList.end(); it++)
    {
        delete *it;
    }
}

/*
线程的回调函数：
1. 如果缓存队列中有任务，则加锁->取出任务，执行任务->开锁。
2. 如果缓存队列中没有任务，使用条件变量等待任务的到来。
3. 首先检查线程的状态。如果关闭线程，即m_isExit为true,则关闭线程。
*/
void ThreadPool::taskProcFunc(void)
{
    pthread_t pid = ::pthread_self();
    while(!m_isExit)
    {
        m_locker.lock();//加锁，对缓存队列长度的锁。m_taskQueue.size()
        //如果缓存队列中没有任务，则等待缓存队列中有任务加入。
        while(m_taskQueue.empty())
        {
            m_locker.wait();
        }
        //如果缓存队列中有任务，则取任务执行。
        ::printf("[tid: %lu] run:", pid);
        Task* task;
        if(!m_taskQueue.empty())
        {
            task = m_taskQueue.front();
            m_taskQueue.pop();
        }
        m_locker.unlock();
        task->function();
        delete task;
        ::printf("[tid: %lu\tidle\n]", pid);
    }

    ::pthread_exit(nullptr);
}

/*
public方法3:停止所有线程：
1. 如果m_isExit的标志已经是true,则退出。
2. 否则：1）唤醒所有等待线程，销毁线程池。
        2）清除僵尸线程。
        3）销毁信号量和互斥量。

*/
int ThreadPool::stopAll()
{
    if(m_isExit) return -1;

    ::printf("Now I will wnd all the threads!");
    m_isExit = true;
    m_locker.wakeAll();

    for(ThreadList::iterator it = m_threadList.begin();
                            it != m_threadList.end(); it++)
    {
        ::pthread_join((*it)->getPid(), NULL);
    }

    return 0;
}
/*
将任务加入线程池。
首先将任务加入任务队列，然后唤醒一个线程处理。
*/
int ThreadPool::addTask(Task* task)
{
    m_locker.lock();
    m_taskQueue.push(task);
    m_locker.unlock();
    m_locker.wakeOne();
    return 0;
}
/*
获得任务队列的大小。
*/
int ThreadPool::getTaskSize()
{
    return m_taskQueue.size();
}
