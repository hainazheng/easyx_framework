#include "FileManager.h"
#include"fstream"


void FileManager::CreateFolder(std::string folderName)
{
	fs::path folderPath = fs::current_path() / folderName;

	if (fs::create_directory(folderPath))  std::cout << "文件夹创建成功: " << folderPath << std::endl;
}

void FileManager::RemoveFolder(std::string folderName)
{
	fs::path folderPath = fs::current_path() / folderName;

	if (fs::remove(folderPath)) std::cout << "文件夹删除成功: " << folderPath << std::endl;
}

void FileManager::CopyFolder(std::string newfolderName, std::string oldfolderName)
{
	fs::path newfolderPath = fs::current_path() / newfolderName;

	fs::path oldfolderPath = fs::current_path() / oldfolderName;

	fs::copy(newfolderPath, oldfolderPath);



	if (fs::exists(newfolderPath)) std::cout << "文件夹复制成功: " << newfolderPath << std::endl;
}

void FileManager::CreateFile(std::string filePath)
{
	fs::path oldPath(fs::current_path() / filePath);
	std::fstream file(oldPath, std::ios::out | std::ios::trunc);
	if (!file)
	{
		std::cout << "Create file({" << oldPath << " }) failed!!!\n";

	}
	file.close();
}

void FileManager::InputFile(std::string filePath, std::string text)
{
	// 判读是否是常规文件，如果是，输出文件大小
	if (fs::is_regular_file(filePath)) {
		std::cout << filePath << "'s size is: " << fs::file_size(filePath) << std::endl;
		std::ofstream outfile;
		outfile.open("filePath");
		outfile << text << std::endl;
	}
	// 若不是常规文件，则可能是目录，就用for循环列出其所有子目录
	else  if (fs::is_directory(filePath)) {
		std::cout << filePath << " is a directory,includes: " << std::endl;
		for (auto& e : fs::directory_iterator(filePath)) {
			std::cout << " " << e.path() << std::endl;
			return;
		}

	}
}

std::string FileManager::ReadFile(std::string filePath)
{
	std::string date;
	// 判读是否是常规文件
	if (fs::is_regular_file(filePath)) {
		std::ifstream infile;
		infile.open("filePath");
		//写入数据
		infile >>date;
		return date;
	}
	// 若不是常规文件，则可能是目录，就用for循环列出其所有子目录
	else  if (fs::is_directory(filePath)) {
		std::cout << filePath << " is a directory,includes: " << std::endl;
		for (auto& e : fs::directory_iterator(filePath)) {
			std::cout << " " << e.path() << std::endl;
			return date;
		}

	}
}


std::string StrKit::Strcat(
	char const* str1,     //[in] 字符串1
	char const* str2,	  //[in] 字符串2
	...					  //[in] 字符串n
)
{
	//用第一个参数字符串初始化结果字符串
	std::string str = str1;
	// 用第二个参数字符串拼接到结果字符串的末尾
	str += str2;

	//变长参数表
	va_list ap;
	//用str2之后的参数初始化变长参数表
	va_start(ap, str2);

	//指向变长参数表中参数的指针
	char const* strx = NULL;
	//依次获取变长参数表中的每个字符串参数
	while (strx = va_arg(ap, char const*))
		//拼接到结尾字符串末尾
		str += strx;

	//销毁变长参数表
	va_end(ap);
	//返回结果字符串
	return str;
}





std::string& StrKit::Trim(
	std::string& str           //[in,out] 待修剪字符串
) {
	//在待修剪字符串中查找第一个和最后一个空白字符串
	std::string::size_type first = str.find_first_not_of(" \t\r\n"),
		last = str.find_last_not_of(" \t\n\r");
	//若没有找到任何非空白字符，说明待修剪字符串
	//要么是空串，要么全部有空白字符组成
	if (first == std::string::npos || last == std::string::npos)
		//清空字符串
		str.clear();
	else
		//截取第一个到最后一个非空白字符之间的子串
		str = str.substr(first, last - first + 1);

	//返回被修剪过的字符串本身
	return str;
}


std::vector<std::string> StrKit::Split(
	std::string const& str,    // [in]待拆分字符串
	std::string const& delim,  // [in]分隔符字符串
	int  limit 		  // [in]拆分次数
) {
	//存放拆分结果的子串向量
	std::vector<std::string> strv;
	//待拆分字符串临时缓冲区，注意多分配一个字符放'\0'

	char* temp = new char[str.size() + 1];

	
	//将带拆分字符串，连同终止空字符（'\0')一起，复制到临时缓冲区中
	strcpy(temp, str.c_str());
	//strtok要求待拆分字符串所在内存必须可写

	//delim : ",.:;()$"
	//limit : 3
	//    temp : The quick,brown.fox:jumps;over(the)lazy$dog/0
	//           t  /t    /t    /t  /t    /t   /t  /t   /t
	//           o  0o    0o    0o  0o    0o   0o  0o   0o
	//           k   k     k     k   k     k    k   k    k
	//           e   e     e     e   e     e    e   e    e
	//           n   n     n     n   n     n    n   n    n
	//    strv : The
	//               quick
	//                     brown
	//                           fox:jumps;over(the)lazy$dog
	// --limit : 2   1     0
	
	//Debug 2024//11/29 strtok(NULL, delim.c_str()) 把null写成temp导致死循环
	//依次提取待拆分字符串中每个被分隔字符串中的字符分隔的子串 注意线程不安全
	for (char* token = strtok(temp, delim.c_str());
		token; token = strtok(NULL, delim.c_str()))
	{
		//拆分出来的子串
		std::string part = token;
		//经修剪后压入存放拆分结果的子串向量
		strv.push_back(Trim(part));
		/*std::cout << part << std::endl;*/

		//若拆分次数限制已到（若limit参数取缺省值0,则
		//此条件永远不可能满足，即不限制拆分次数），且本次
		//所拆并非最后一个子串
		if (!--limit && (token += strlen(token)) - temp > (int)str.size())
		{
			//将待拆分字符串的其余部分一次性
			// 压入存放拆分结果的子串向量
			part = (++token);
			strv.push_back(Trim(part));
			//提前结束拆分循环
			break;
		}

	}
	//释放开辟的动态缓冲区
	delete[] temp;
	//返回存放拆分结果的子串向量
	return strv;
}