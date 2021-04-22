#ifndef THREAD_OO_H
#define THREAD_OO_H

#include <list>
#include <pthread.h>
#include <unistd.h>
#include <functional>

typedef std::function<void(void)> RunCallBack;

///
/// \brief 简易的线程类
/// \details 使用基于对象的方法
class Thread
{
public:
    Thread(RunCallBack runFunc);
    ~Thread();

    void start();
    void join();
    pthread_t getPid(){return m_pid;}

    static void* runInThread(void *arg);
public:
    bool m_isJoin;
    bool m_isStart;
    pthread_t m_pid;
    RunCallBack m_runFunc;
};

typedef std::list<Thread *> ThreadList;

#endif // THREAD_OO_H
