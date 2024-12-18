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
        std::unique_lock<std::mutex> lock(m_Mutex); // ���� һ�����ö�����ͷ� ��ʱ����
        if (!cfg)
        {
            cfg = new Configurator;
        }
    }
    return cfg;
}

Configurator::Configurator(void) :m_logLevel(Log::LEVEL_DBG)    //��С��־�ȼ�ȱʡΪ����
{

}

//��ʼ��
void Configurator::Initialize()
{
    load(std::string("Configurator.cfg"));
}


void Configurator::load(
    std::string const& cfgFile /// < [in] �����ļ�·��
)
{
    //����·���������ļ�������
    std::ifstream ifs(cfgFile.c_str());
    //��ʧ��
    if (!ifs)

        //��¼һ������ռ�
        Log::instance()->printf(Log::LEVEL_ERR, __FILE__, __LINE__,
            "���������ļ�\"%s\"ʧ��",cfgFile.c_str());

 
    //�ļ����ַ���
    std::string line;
    //���ж�ȡ�����ļ�
    for (int lineNo = 0; getline(ifs, line); ++lineNo)
    {
        //�޼����ַ���
        StrKit::Trim(line);
        //��Ϊע���л��߿���
        if (line[0] == '\0' || line[0] == '#')
            continue;
        //������ַ������ԵȺ�Ϊ�ָ��������������
        std::vector<std::string> strv = StrKit::Split(line, "=", 2);

        //���ɹ���ֳ�����ֵ
        if (strv.size() == 2)
        {
            //����Ϊ"LOG_LEVEL"
            if (!stricmp(strv[0].c_str(), "LOG_LEVEL"))
                //��ֵΪ"�����־�ȼ�"
                m_logLevel = atoi(strv[1].c_str());
            //����
            else
                //����Ϊ��LOG_FILE"
                if (!stricmp(strv[0].c_str(), "LOG_FILE"))
                    //��ֵΪ"��־�ļ�·��"
                    m_logfile = strv[1];
        }
    }
    // �ر������ļ�������
    ifs.close();
}