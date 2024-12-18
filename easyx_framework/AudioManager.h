/*****************************************************************//**
 * \file   AudioManager.h
 * \brief  ȫ����Ƶ������
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"
#include<unordered_map>
#include<map>

//��Ƶ��Ϣ
//struct AudioInfo
//{
//	std::string name;
//	std::string path;
//	//int index;		//�±�
//};

//ʵ���±�����º���
//struct IndexSort
//{
//	bool operator()(const std::pair<std::string, int> a, const std::pair<std::string, int> b)const;
//};


/** 
 * ȫ����Ƶ���� ��������ģʽ
 * @brief ֻ֧�ֲ��ű������� 
 * һ��һ��.
 * 
 * @see ResourceManager
 */
class AudioManager
{
public:
	static AudioManager* instance();
	
	void Initialize();

	
	/*std::unordered_set<std::string> TrversalAll();*/

	//����������С
	void SetBGVolume(int vol);

	//��������
	void Play(std::string name, bool repeat = false);

	

	//����������Դ����ʡ�ڴ棬���ٴβ��Ż����Ľϴ����ܣ�
	void Stop();

	//ɾ������
	void Remove(std::string name);

	//�����������
	void SetRandom(bool random);

	void NextSound();

	//��ͣ����
	void Pause();

	//��������
	void Resume();

	//��������
	void Load(std::string name);

	//void Load();
private:
	static AudioManager* manager;

	static std::mutex m_Mutex;

	AudioManager() {};

	~AudioManager();

	std::unordered_map< std::string, std::string >paths;		///<�����ַ���·��
	std::unordered_map< std::string,int >indexmap;		///<ʵ�ֲ���˳��Ĵ洢 ֵ�������

	std::string currentpath="null";			///<��ǰ��������·��

	///���´��� ����֮�����һ���±�
	int UpdateIndex();
	
	void Play( bool repeat = false);
	
	///�رյ�ǰ���� �ͷ���Դ �ڲ�����
	void Close();

	///�л���һ������
	void Next();
	
	int SettingVol = 1000;		///<ȫ������ϵ�� ԭʼ0~1000 ӳ�䵽0~SettingVol
	
	int BGVol = 0;			///<��������

	bool repeat=false;			///<�Ƿ��ظ�����

	bool random = false;		///<�Ƿ��������

	bool next = false;		///<�ⲿ����nextֻ�����÷���λ
};

