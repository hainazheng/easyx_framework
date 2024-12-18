/*****************************************************************//**
 * \file   Configurator.h
 * \brief  配置器
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"string"
#include"CoreMinimal.h"

/**
 * 配置器
 * 懒汉模式.
 */
class Configurator
{
    static Configurator* cfg;

    static std::mutex m_Mutex;

    
    /// @brief 构造
    Configurator(void);

    ~Configurator() {};
public:
    static Configurator* instance();

    void Initialize();
    
    /// @brief 从指定的配置文件中加载配置信息
    void load(
        std::string const& cfgFile /// < [in] 配置文件路径
    );

    /// @brief 最低日志等级 \brief
    ///从低到高依次为：
    /// - 0 - 调试
    /// - 1 - 信息
    /// - 2 - 警告
    /// - 3 - 错误
    /// - 4 - 致命
    /// 系统将记录所有不低于指定等级的日志信息
    int m_logLevel;
    /// @brief 日志文件路径
    std::string m_logfile;
 


};
