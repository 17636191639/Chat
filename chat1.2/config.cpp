#include "config.h"

//#include <sys/types.h>
//#include <sys/stat.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <signal.h>
//#include <fcntl.h>
//#include <sys/resource.h>  /*setrlimit */

//void Config::init_signal(void)
//{
//    signal(SIGCHLD, SIG_DFL);
//    signal(SIGPIPE, SIG_IGN);
//}

//int Config::set_fdlimit()
//{
//    struct rlimit rt;
//    rt.rlim_max = rt.rlim_cur = MAXCONN;
//    if (setrlimit(RLIMIT_NOFILE, &rt) == -1)
//    {
//        perror("setrlimit error");
//        return -1;
//    }

//    return 0;
//}

//bool Config::daemon_run_method() //Linux高性能服务器编程.pdf,游双
//{
//    //创建子进程，关闭父进程，这样可以使程序在后台进行
//    pid_t pid = fork();
//    if ( pid < 0 )
//    {
//        return false;
//    }
//    else if ( pid > 0 )
//    {
//        exit( 0 );
//    }

//    //设置文件权限掩码。当进程创建新文件时，文件的权限将是mode & 0777
//    umask(0);

//    //创建新的会话，设置本进程为进程组的首领
//    pid_t sid = setsid();
//    if ( sid < 0 )
//    {
//        return false;
//    }
//    //切换工作目录
//    if ( ( chdir( "/" ) ) < 0 )
//    {
//        /* Log the failure */
//        return false;
//    }
//    //关闭标准输入设备、标准输出设备和标准错误输出设备
//    close( STDIN_FILENO );
//    close( STDOUT_FILENO );
//    close( STDERR_FILENO );
//    //将标准输入、输出和错误输出都定向到/dev/null文件
//    open( "/dev/null", O_RDONLY );
//    open( "/dev/null", O_RDWR );
//    open( "/dev/null", O_RDWR );
//    return true;
//}
