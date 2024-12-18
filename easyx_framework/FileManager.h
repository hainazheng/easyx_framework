/*****************************************************************//**
 * \file   FileManager.h
 * \brief  文件管理器、字符串工具包
 * 
 * \author zhn
 *********************************************************************/

#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include<string>

//c库
#include<stdarg.h>

#pragma warning (disable:4996)


namespace fs = std::filesystem;
//using std::string;


/**
 * 文件管理器.
 */
class FileManager
{
public:
	/// @brief 创建文件夹
	static void CreateFolder(
		std::string folderPath			///[in]相对路径 或者绝对路径
	);

	/// @brief 移除文件夹
	static void RemoveFolder(
		std::string folderPath			///[in]相对路径 或者绝对路径
	);

	/// @brief 复制文件夹
	
	static void CopyFolder(
		std::string newfolderPath		///@param [in]新文件相对路径 或者绝对路径
		,std::string oldfolderPath		///@param [in]旧文件相对路径 或者绝对路径
	);

	
	/// @brief 创建文件
	/// @remark 相对路径
	static void CreateFile(
		std::string filePath		/// [in] 新文件相对路径
	);

	
	/// @brief 写文件
	/// @remark 默认已经处理好字符 最后换行 如果不是文件而是文件夹只会显示子文件
	static void InputFile(
		std::string filePath,		///[in]新文件相对路径 或者绝对路径
		std::string text			///[in]内容
	);

	/// @brief 读文件
	/// @return 读取的数据
	/// @remark 不做处理 如果不是文件而是文件夹只会显示子文件
	static std::string ReadFile(
		std::string filePath		///[in]新文件相对路径 或者绝对路径
	);
};


/**
 * 字符串工具包.
 */
class StrKit {
public:

	/// @brief 字符串拼接
	/// @return 拼接后的字符串
	static std::string Strcat(
		char const* str1 ///< [in] 字符串1
		,char const* str2 ///< [in] 字符串2
		,...               ///< [in] 字符串n
	);

	/// @brief 字符串修剪
	/// @return 被修剪过的参数字符串本身
	/// @remark 截去字符串的首尾空白字符(空格、制表、回车、换行等)
	static std::string& Trim(
		std::string& str ///< [in,out] 待修剪字符串
	);

	/// @brief 字符串拆分
	/// @return 被拆分出的子串向量
	/// @remark 以delim中的字符作为分隔符，对str字符串进行拆分，并对每个被拆分出的子串做修剪，拆分次数不超过limit，除非该参数的值为0
	static std::vector<std::string> Split(
		std::string const& str      ///< [in] 待拆分字符串
		,std::string const& delim   ///< [in] 分隔符字符串
		, int           limit = 0 ///< [in] 拆分次数限制
	);


};