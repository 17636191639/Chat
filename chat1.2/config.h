#ifndef CONFIG_H
#define CONFIG_H

#define IPADDRESS   "0.0.0.0"
#define MAXSIZE     1024
#define LISTENQ     512
#define FDSIZE      5
#define EPOLLEVENTS 60000
#define MAXCONN     60000

class Config
{
public:
    //设置信号处理,SIG_IGN表示忽略信号,SIG_DFL表示使用信号的默认处理方式
    static void init_signal(void);

    //设置每个进程允许打开的最大文件数
    //这项功能等价于linux终端命令 "ulimit -n 102400"
    static int set_fdlimit();

    //創建守護進程
    //Linux高性能服务器编程.pdf
    static bool daemon_run_method();
};

#endif // CONFIG_H
