#include"Configurator.h"
#include "Log.h"
#include<fstream>
#include "FileManager.h"
#pragma warning (disable:4996)

Configurator* Configurator::cfg = nullptr;

std::mutex Configurator::m_Mutex;

Configurator* Configurator::instance()
{
    if (!cfg)
    {
        std::unique_lock<std::mutex> lock(m_Mutex); // 加锁 一创建好对象就释放 耗时操作
        if (!cfg)
        {
            cfg = new Configurator;
        }
    }
    return cfg;
}

Configurator::Configurator(void) :m_logLevel(Log::LEVEL_DBG)    //最小日志等级缺省为调试
{

}

//初始化
void Configurator::Initialize()
{
    load(std::string("Configurator.cfg"));
}


void Configurator::load(
    std::string const& cfgFile /// < [in] 配置文件路径
)
{
    //根据路径打开配置文件输入流
    std::ifstream ifs(cfgFile.c_str());
    //若失败
    if (!ifs)

        //记录一般错误日记
        Log::instance()->printf(Log::LEVEL_ERR, __FILE__, __LINE__,
            "加载配置文件\"%s\"失败",cfgFile.c_str());

 
    //文件行字符串
    std::string line;
    //逐行读取配置文件
    for (int lineNo = 0; getline(ifs, line); ++lineNo)
    {
        //修剪行字符串
        StrKit::Trim(line);
        //若为注释行或者空行
        if (line[0] == '\0' || line[0] == '#')
            continue;
        //拆分行字符串，以等号为分隔符，最多拆分两次
        std::vector<std::string> strv = StrKit::Split(line, "=", 2);

        //若成功拆分出键和值
        if (strv.size() == 2)
        {
            //若键为"LOG_LEVEL"
            if (!stricmp(strv[0].c_str(), "LOG_LEVEL"))
                //其值为"最低日志等级"
                m_logLevel = atoi(strv[1].c_str());
            //否则
            else
                //若键为”LOG_FILE"
                if (!stricmp(strv[0].c_str(), "LOG_FILE"))
                    //其值为"日志文件路径"
                    m_logfile = strv[1];
        }
    }
    // 关闭配置文件输入流
    ifs.close();
}