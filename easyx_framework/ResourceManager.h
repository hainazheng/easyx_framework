/*****************************************************************//**
 * \file   ResourceManager.h
 * \brief  ��Դ������
 * 
 * \author zhn
 *********************************************************************/


#pragma once
#include"easyx.h"
#include"string"
#include"unordered_map"
#include "CoreMinimal.h"


///ͼƬ������Դ����
struct AnimationResource
{
	IMAGE **images;			///<[in]����ͼƬ�洢����
	int num;		///<[in]�����ܹ�֡��
};

///��Դ������
///��������ģʽ
/// @see AudioManager
class ResourceManager
{
	friend class World;
	friend class AudioManager;
private:
	static ResourceManager* manager;			///<����ָ��
	static std::mutex m_Mutex;					///<��

	ResourceManager() {};
	~ResourceManager() {};

	std::unordered_map<std::string, IMAGE*> ImagePool;		///<ͼƬ��	[ͼƬ����,ͼ����]
	std::unordered_map<std::string, AnimationResource>  animationPool;			///<������	[�������֣�������Դ��]
	std::unordered_map<std::string,std::string>  AudioPool;			///<��Ƶ��		[��Ƶ���֣�·��]
	
	///���ص���ͼƬ
	void Load(std::string name, std::string path, int wid, int hei);	
	///���ض��Ŷ���ͼƬ
	/// @remark ·�����ļ� ���üӽ�β"/"
	void Load(std::string name, std::string path, int wid, int hei, int num);	
	/// ���ض��Ŷ���ͼƬ 
	/// @remark ÿһ��ͼƬ��Ҫָ����С������
	void Load(std::string name, std::string path, const std::vector<Pair>& arr);
	///����Springsheet����ͼƬ
	void Load(std::string name, std::string path, int wid, int hei, int num, int row, int col);		
	///������Ƶý����Դ
	void Load(std::string name, std::string path);		
public:
	void Initialize();

	///��ȡʵ��
	static ResourceManager* instance();
	
	///�������ַ���ͼƬ������ӦͼƬָ��
	IMAGE* Fetch(
		std::string name			///<[in]ͼƬ����
	);		

	///�������ַ��ض���������Ӧ������Դ
	AnimationResource AnimationFetch(
		std::string name			///<[in]��������
	);			

	///��Ƶ�Ƿ����
	bool IsAudio(
		std::string name			///<[in]��Ƶ����
	);
};