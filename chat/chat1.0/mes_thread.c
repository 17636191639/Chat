#include "mes_thread.h"
#include <stdio.h>


//void mes_thread_init(mes_thread *thread, pfun callback_fun)
//{
//    thread->is_start = 0;
//    thread->callback_fun = callback_fun;

//}
//void thread_start(mes_thread *thread)
//{
//    if(!thread->is_start)
//    {
//        thread->is_start = 1;
//        int res = pthread_create(thread->pid, NULL, (void*)thread_run, NULL);
//        if(res != 0)
//        {
//            printf("Create pthread error!\n");
//            return ;
//        }
//    }

//}
//void thread_run(mes_thread *thread) //线程开始被自动调用
//{

//    if(thread != NULL)
//    {
//        if(thread->is_start)
//        {
//            thread->callback_fun();
//        }
//    }
//}
