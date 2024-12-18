/*****************************************************************//**
 * \file   Log.h
 * \brief  日志类
 *
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"


 /// @brief 日志类 
 /// 单例懒汉模式
 /// @see Configurator
class Log
{
public:

	static Log* instance();



	/// @brief 日志等级 - 调试
	static int const LEVEL_DBG = 0;
	/// @brief 日志等级 - 信息 
	static int const LEVEL_INF = 1;
	/// @brief 日志等级 - 警告
	static int const LEVEL_WAR = 2;
	/// @brief 日志等级 - 一般错误
	static int const LEVEL_ERR = 3;
	/// @brief 日志等级 - 严重错误
	static int const LEVEL_CRT = 4;

	///@brief 按格式打印日志
	///@remark想要输入到相应的日志文件就要重定向stdout
	void printf(
		int         level		///< [in]日志等级
		, char const* file		///< [in]源码文件
		, int         line		///< [in]源码行号
		, char const* format    ///< [in]格式化串
		, ...                   ///< [in]打印内容、数量和类型和格式化串format中的格式化标记对应匹配
	)const;
private:

	static Log* log;			///<日志类实例

	static std::mutex m_Mutex;	///<锁

	Log() {};

	~Log() {};

	///@brief 日志等级标签数组 \brief
	///从低到高依次为：
	/// - deb - 调试
	/// - inf - 信息
	/// - war - 警告
	/// - err - 一般错误
	/// - crt - 致命错误
	static char const* s_levels[];
};

