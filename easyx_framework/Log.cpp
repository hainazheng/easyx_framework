
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

///��ȡʵ��
Log* Log::instance()
{
    if (!log)
    {
        std::unique_lock<std::mutex> lock(m_Mutex); // ���� һ�����ö�����ͷ� ��ʱ����
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
    // ����־�ȼ������������ļ��е�"�����־�ȼ�"
    if (level >= mainWorld.m_cfg->m_logLevel) {
        // ��ʽ����ǰϵͳ���ں�ʱ���ַ���
        /*char dateTime[32];
        time_t now = time(NULL);
        strftime(dateTime, sizeof(dateTime),
            "%Y-%m-%d %H:%M:%S", localtime(&now));*/
        std::thread::id this_thread_id = std::this_thread::get_id(); // ��ȡ���̵߳�id
        std::string dateTime=GetRealTime(6);
        // ��ӡ��־ͷ��
        // [����ʱ��][��־�ȼ�][pid=���̱�ʶ tid=�̱߳�ʶ][�ļ�:�к�]
        fprintf(stdout, "[%s][%s][pid=%d tid=%lu][%s:%d]\n",
            dateTime.c_str(), s_levels[level], GetCurrentProcessId(), this_thread_id,
            file, line);

        //// ��ӡ��־ͷ��
        //// [����ʱ��][��־�ȼ�][pid=���̱�ʶ tid=�̱߳�ʶ][�ļ�:�к�]
        //fprintf(stdout, "[%s][%s][pid=%d tid=%lu][%s:%d]\n",
        //    dateTime, s_levels[level], getpid(), pthread_self(),
        //    file, line);

        // �䳤������
        va_list ap;
        // ��format�Ժ�Ĳ�����ʼ���䳤������
        va_start(ap, format);
        // ��format��ʽ��ӡ�䳤�������е����� Ĭ��stdout�ǿ���̨
        vfprintf(stdout, format, ap);
        // ���ٱ䳤������
        va_end(ap);

        // ���в���ӡ����
        fprintf(stdout, "\n\n");
    }

    // ����־�ȼ�������һ�����
    if (level >= LEVEL_ERR)
        // ��ǰ��ֹ����
        exit(EXIT_FAILURE);
}
/// ��־�ȼ���ǩ����
/// �ӵ͵�������Ϊ��
/// dbg - ����
/// inf - ��Ϣ
/// war - ����
/// err - һ�����
/// crt - ��������
char const* Log::s_levels[] = { "dbg", "inf", "war", "err", "crt" };