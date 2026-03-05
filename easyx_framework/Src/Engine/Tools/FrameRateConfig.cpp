#include"ZHNpch.h"
#include "FrameRateConfig.h"


FrameRateConfig* FrameRateConfig::Frc = nullptr;

std::mutex FrameRateConfig::m_Mutex;

FrameRateConfig* FrameRateConfig::instance()
{
    if (!Frc)
    {
        std::unique_lock<std::mutex> lock(m_Mutex); // 加锁 一创建好对象就释放 耗时操作
        Frc = new FrameRateConfig;
    }
    return Frc;
}

FrameRateConfig::~FrameRateConfig()
{
}
