/*****************************************************************//**
 * \file   FileManager.h
 * \brief  �ļ����������ַ������߰�
 * 
 * \author zhn
 *********************************************************************/

#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include<string>

//c��
#include<stdarg.h>

#pragma warning (disable:4996)


namespace fs = std::filesystem;
//using std::string;


/**
 * �ļ�������.
 */
class FileManager
{
public:
	/// @brief �����ļ���
	static void CreateFolder(
		std::string folderPath			///[in]���·�� ���߾���·��
	);

	/// @brief �Ƴ��ļ���
	static void RemoveFolder(
		std::string folderPath			///[in]���·�� ���߾���·��
	);

	/// @brief �����ļ���
	
	static void CopyFolder(
		std::string newfolderPath		///@param [in]���ļ����·�� ���߾���·��
		,std::string oldfolderPath		///@param [in]���ļ����·�� ���߾���·��
	);

	
	/// @brief �����ļ�
	/// @remark ���·��
	static void CreateFile(
		std::string filePath		/// [in] ���ļ����·��
	);

	
	/// @brief д�ļ�
	/// @remark Ĭ���Ѿ�������ַ� ����� ��������ļ������ļ���ֻ����ʾ���ļ�
	static void InputFile(
		std::string filePath,		///[in]���ļ����·�� ���߾���·��
		std::string text			///[in]����
	);

	/// @brief ���ļ�
	/// @return ��ȡ������
	/// @remark �������� ��������ļ������ļ���ֻ����ʾ���ļ�
	static std::string ReadFile(
		std::string filePath		///[in]���ļ����·�� ���߾���·��
	);
};


/**
 * �ַ������߰�.
 */
class StrKit {
public:

	/// @brief �ַ���ƴ��
	/// @return ƴ�Ӻ���ַ���
	static std::string Strcat(
		char const* str1 ///< [in] �ַ���1
		,char const* str2 ///< [in] �ַ���2
		,...               ///< [in] �ַ���n
	);

	/// @brief �ַ����޼�
	/// @return ���޼����Ĳ����ַ�������
	/// @remark ��ȥ�ַ�������β�հ��ַ�(�ո��Ʊ��س������е�)
	static std::string& Trim(
		std::string& str ///< [in,out] ���޼��ַ���
	);

	/// @brief �ַ������
	/// @return ����ֳ����Ӵ�����
	/// @remark ��delim�е��ַ���Ϊ�ָ�������str�ַ������в�֣�����ÿ������ֳ����Ӵ����޼�����ִ���������limit�����Ǹò�����ֵΪ0
	static std::vector<std::string> Split(
		std::string const& str      ///< [in] ������ַ���
		,std::string const& delim   ///< [in] �ָ����ַ���
		, int           limit = 0 ///< [in] ��ִ�������
	);


};