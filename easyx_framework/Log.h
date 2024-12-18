/*****************************************************************//**
 * \file   Log.h
 * \brief  ��־��
 *
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"


 /// @brief ��־�� 
 /// ��������ģʽ
 /// @see Configurator
class Log
{
public:

	static Log* instance();



	/// @brief ��־�ȼ� - ����
	static int const LEVEL_DBG = 0;
	/// @brief ��־�ȼ� - ��Ϣ 
	static int const LEVEL_INF = 1;
	/// @brief ��־�ȼ� - ����
	static int const LEVEL_WAR = 2;
	/// @brief ��־�ȼ� - һ�����
	static int const LEVEL_ERR = 3;
	/// @brief ��־�ȼ� - ���ش���
	static int const LEVEL_CRT = 4;

	///@brief ����ʽ��ӡ��־
	///@remark��Ҫ���뵽��Ӧ����־�ļ���Ҫ�ض���stdout
	void printf(
		int         level		///< [in]��־�ȼ�
		, char const* file		///< [in]Դ���ļ�
		, int         line		///< [in]Դ���к�
		, char const* format    ///< [in]��ʽ����
		, ...                   ///< [in]��ӡ���ݡ����������ͺ͸�ʽ����format�еĸ�ʽ����Ƕ�Ӧƥ��
	)const;
private:

	static Log* log;			///<��־��ʵ��

	static std::mutex m_Mutex;	///<��

	Log() {};

	~Log() {};

	///@brief ��־�ȼ���ǩ���� \brief
	///�ӵ͵�������Ϊ��
	/// - deb - ����
	/// - inf - ��Ϣ
	/// - war - ����
	/// - err - һ�����
	/// - crt - ��������
	static char const* s_levels[];
};

