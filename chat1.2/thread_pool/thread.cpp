#include "thread.h"
#include <iostream>

Thread::Thread(RunCallBack runFunc)
{
    m_pid = 0;
    m_isJoin = false;
    m_isStart = false;
    m_runFunc = std::move(runFunc);
}

Thread::~Thread()
{
    if(m_isStart && !m_isJoin)
    {
        ::pthread_detach(m_pid);
    }

}

void Thread::start()
{
    if(!m_isStart)
    {
        m_isStart = true;
        ::pthread_create(&m_pid, NULL, runInThread, this);
    }
}

void Thread::join()
{
    if(m_isStart)
    {
        m_isJoin = true;
        ::pthread_join(m_pid,NULL);
    }
}

void* Thread::runInThread(void* arg)
{
    Thread* p = static_cast<Thread*>(arg);
    if(NULL != p)
    {
        p->m_runFunc();
    }

    return nullptr;
}

