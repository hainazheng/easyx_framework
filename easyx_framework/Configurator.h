/*****************************************************************//**
 * \file   Configurator.h
 * \brief  ������
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"string"
#include"CoreMinimal.h"

/**
 * ������
 * ����ģʽ.
 */
class Configurator
{
    static Configurator* cfg;

    static std::mutex m_Mutex;

    
    /// @brief ����
    Configurator(void);

    ~Configurator() {};
public:
    static Configurator* instance();

    void Initialize();
    
    /// @brief ��ָ���������ļ��м���������Ϣ
    void load(
        std::string const& cfgFile /// < [in] �����ļ�·��
    );

    /// @brief �����־�ȼ� \brief
    ///�ӵ͵�������Ϊ��
    /// - 0 - ����
    /// - 1 - ��Ϣ
    /// - 2 - ����
    /// - 3 - ����
    /// - 4 - ����
    /// ϵͳ����¼���в�����ָ���ȼ�����־��Ϣ
    int m_logLevel;
    /// @brief ��־�ļ�·��
    std::string m_logfile;
 


};
