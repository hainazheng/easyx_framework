
#include"Log.h"
#include"stdarg.h"
#include"Configurator.h"
#include<thread>
#include"World.h"
//#include <processthreadsapi.h>
#ifndef __linux__
#include "windows.h"
#else
#include "unistd.h"
#endif




Log* Log::log = nullptr;

std::mutex Log::m_Mutex;

///获取实例
Log* Log::instance()
{
    if (!log)
    {
        std::unique_lock<std::mutex> lock(m_Mutex); // 加锁 一创建好对象就释放 耗时操作
        if (!log)
        {
            log = new Log;
        }
    }
    return log;
}


void Log::printf(
    int         level,    
    char const* file,    
    int         line,     
    char const* format,   
    ...                   
)const
{
    // 若日志等级不低于配置文件中的"最低日志等级"
    if (level >= mainWorld.m_cfg->m_logLevel) {
        // 格式化当前系统日期和时间字符串
        /*char dateTime[32];
        time_t now = time(NULL);
        strftime(dateTime, sizeof(dateTime),
            "%Y-%m-%d %H:%M:%S", localtime(&now));*/
        std::thread::id this_thread_id = std::this_thread::get_id(); // 获取本线程的id
        std::string dateTime=GetRealTime(6);
        // 打印日志头：
        // [日期时间][日志等级][pid=进程标识 tid=线程标识][文件:行号]
        fprintf(stdout, "[%s][%s][pid=%d tid=%lu][%s:%d]\n",
            dateTime.c_str(), s_levels[level], GetCurrentProcessId(), this_thread_id,
            file, line);

        //// 打印日志头：
        //// [日期时间][日志等级][pid=进程标识 tid=线程标识][文件:行号]
        //fprintf(stdout, "[%s][%s][pid=%d tid=%lu][%s:%d]\n",
        //    dateTime, s_levels[level], getpid(), pthread_self(),
        //    file, line);

        // 变长参数表
        va_list ap;
        // 用format以后的参数初始化变长参数表
        va_start(ap, format);
        // 按format格式打印变长参数表中的内容 默认stdout是控制台
        vfprintf(stdout, format, ap);
        // 销毁变长参数表
        va_end(ap);

        // 换行并打印空行
        fprintf(stdout, "\n\n");
    }

    // 若日志等级不低于一般错误
    if (level >= LEVEL_ERR)
        // 提前终止进程
        exit(EXIT_FAILURE);
}
/// 日志等级标签数组
/// 从低到高依次为：
/// dbg - 调试
/// inf - 信息
/// war - 警告
/// err - 一般错误
/// crt - 致命错误
char const* Log::s_levels[] = { "dbg", "inf", "war", "err", "crt" };