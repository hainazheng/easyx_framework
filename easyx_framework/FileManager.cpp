#include "FileManager.h"
#include"fstream"


void FileManager::CreateFolder(std::string folderName)
{
	fs::path folderPath = fs::current_path() / folderName;

	if (fs::create_directory(folderPath))  std::cout << "�ļ��д����ɹ�: " << folderPath << std::endl;
}

void FileManager::RemoveFolder(std::string folderName)
{
	fs::path folderPath = fs::current_path() / folderName;

	if (fs::remove(folderPath)) std::cout << "�ļ���ɾ���ɹ�: " << folderPath << std::endl;
}

void FileManager::CopyFolder(std::string newfolderName, std::string oldfolderName)
{
	fs::path newfolderPath = fs::current_path() / newfolderName;

	fs::path oldfolderPath = fs::current_path() / oldfolderName;

	fs::copy(newfolderPath, oldfolderPath);



	if (fs::exists(newfolderPath)) std::cout << "�ļ��и��Ƴɹ�: " << newfolderPath << std::endl;
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
	// �ж��Ƿ��ǳ����ļ�������ǣ�����ļ���С
	if (fs::is_regular_file(filePath)) {
		std::cout << filePath << "'s size is: " << fs::file_size(filePath) << std::endl;
		std::ofstream outfile;
		outfile.open("filePath");
		outfile << text << std::endl;
	}
	// �����ǳ����ļ����������Ŀ¼������forѭ���г���������Ŀ¼
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
	// �ж��Ƿ��ǳ����ļ�
	if (fs::is_regular_file(filePath)) {
		std::ifstream infile;
		infile.open("filePath");
		//д������
		infile >>date;
		return date;
	}
	// �����ǳ����ļ����������Ŀ¼������forѭ���г���������Ŀ¼
	else  if (fs::is_directory(filePath)) {
		std::cout << filePath << " is a directory,includes: " << std::endl;
		for (auto& e : fs::directory_iterator(filePath)) {
			std::cout << " " << e.path() << std::endl;
			return date;
		}

	}
}


std::string StrKit::Strcat(
	char const* str1,     //[in] �ַ���1
	char const* str2,	  //[in] �ַ���2
	...					  //[in] �ַ���n
)
{
	//�õ�һ�������ַ�����ʼ������ַ���
	std::string str = str1;
	// �õڶ��������ַ���ƴ�ӵ�����ַ�����ĩβ
	str += str2;

	//�䳤������
	va_list ap;
	//��str2֮��Ĳ�����ʼ���䳤������
	va_start(ap, str2);

	//ָ��䳤�������в�����ָ��
	char const* strx = NULL;
	//���λ�ȡ�䳤�������е�ÿ���ַ�������
	while (strx = va_arg(ap, char const*))
		//ƴ�ӵ���β�ַ���ĩβ
		str += strx;

	//���ٱ䳤������
	va_end(ap);
	//���ؽ���ַ���
	return str;
}





std::string& StrKit::Trim(
	std::string& str           //[in,out] ���޼��ַ���
) {
	//�ڴ��޼��ַ����в��ҵ�һ�������һ���հ��ַ���
	std::string::size_type first = str.find_first_not_of(" \t\r\n"),
		last = str.find_last_not_of(" \t\n\r");
	//��û���ҵ��κηǿհ��ַ���˵�����޼��ַ���
	//Ҫô�ǿմ���Ҫôȫ���пհ��ַ����
	if (first == std::string::npos || last == std::string::npos)
		//����ַ���
		str.clear();
	else
		//��ȡ��һ�������һ���ǿհ��ַ�֮����Ӵ�
		str = str.substr(first, last - first + 1);

	//���ر��޼������ַ�������
	return str;
}


std::vector<std::string> StrKit::Split(
	std::string const& str,    // [in]������ַ���
	std::string const& delim,  // [in]�ָ����ַ���
	int  limit 		  // [in]��ִ���
) {
	//��Ų�ֽ�����Ӵ�����
	std::vector<std::string> strv;
	//������ַ�����ʱ��������ע������һ���ַ���'\0'

	char* temp = new char[str.size() + 1];

	
	//��������ַ�������ͬ��ֹ���ַ���'\0')һ�𣬸��Ƶ���ʱ��������
	strcpy(temp, str.c_str());
	//strtokҪ�������ַ��������ڴ�����д

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
	
	//Debug 2024//11/29 strtok(NULL, delim.c_str()) ��nullд��temp������ѭ��
	//������ȡ������ַ�����ÿ�����ָ��ַ����е��ַ��ָ����Ӵ� ע���̲߳���ȫ
	for (char* token = strtok(temp, delim.c_str());
		token; token = strtok(NULL, delim.c_str()))
	{
		//��ֳ������Ӵ�
		std::string part = token;
		//���޼���ѹ���Ų�ֽ�����Ӵ�����
		strv.push_back(Trim(part));
		/*std::cout << part << std::endl;*/

		//����ִ��������ѵ�����limit����ȡȱʡֵ0,��
		//��������Զ���������㣬�������Ʋ�ִ��������ұ���
		//���𲢷����һ���Ӵ�
		if (!--limit && (token += strlen(token)) - temp > (int)str.size())
		{
			//��������ַ��������ಿ��һ����
			// ѹ���Ų�ֽ�����Ӵ�����
			part = (++token);
			strv.push_back(Trim(part));
			//��ǰ�������ѭ��
			break;
		}

	}
	//�ͷſ��ٵĶ�̬������
	delete[] temp;
	//���ش�Ų�ֽ�����Ӵ�����
	return strv;
}