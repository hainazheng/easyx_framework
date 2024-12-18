/*****************************************************************//**
 * \file   AudioManager.h
 * \brief  全局音频管理器
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"
#include<unordered_map>
#include<map>

//音频信息
//struct AudioInfo
//{
//	std::string name;
//	std::string path;
//	//int index;		//下标
//};

//实现下标排序仿函数
//struct IndexSort
//{
//	bool operator()(const std::pair<std::string, int> a, const std::pair<std::string, int> b)const;
//};


/** 
 * 全局音频管理 单例懒汉模式
 * @brief 只支持播放背景音乐 
 * 一次一首.
 * 
 * @see ResourceManager
 */
class AudioManager
{
public:
	static AudioManager* instance();
	
	void Initialize();

	
	/*std::unordered_set<std::string> TrversalAll();*/

	//设置音量大小
	void SetBGVolume(int vol);

	//播放音乐
	void Play(std::string name, bool repeat = false);

	

	//回收音乐资源（节省内存，但再次播放会消耗较大性能）
	void Stop();

	//删除音乐
	void Remove(std::string name);

	//设置随机播放
	void SetRandom(bool random);

	void NextSound();

	//暂停音乐
	void Pause();

	//继续音乐
	void Resume();

	//加载音乐
	void Load(std::string name);

	//void Load();
private:
	static AudioManager* manager;

	static std::mutex m_Mutex;

	AudioManager() {};

	~AudioManager();

	std::unordered_map< std::string, std::string >paths;		///<由名字访问路径
	std::unordered_map< std::string,int >indexmap;		///<实现播放顺序的存储 值代表次序

	std::string currentpath="null";			///<当前播放音乐路径

	///更新次序 返回之后的下一个下标
	int UpdateIndex();
	
	void Play( bool repeat = false);
	
	///关闭当前音乐 释放资源 内部管理
	void Close();

	///切换下一首音乐
	void Next();
	
	int SettingVol = 1000;		///<全局音量系数 原始0~1000 映射到0~SettingVol
	
	int BGVol = 0;			///<背景音量

	bool repeat=false;			///<是否重复播放

	bool random = false;		///<是否随机播放

	bool next = false;		///<外部访问next只是设置符号位
};

