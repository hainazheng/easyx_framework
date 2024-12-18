/*****************************************************************//**
 * \file   ResourceManager.h
 * \brief  资源管理器
 * 
 * \author zhn
 *********************************************************************/


#pragma once
#include"easyx.h"
#include"string"
#include"unordered_map"
#include "CoreMinimal.h"


///图片动画资源类型
struct AnimationResource
{
	IMAGE **images;			///<[in]加载图片存储对象
	int num;		///<[in]动画总共帧数
};

///资源管理器
///单例懒汉模式
/// @see AudioManager
class ResourceManager
{
	friend class World;
	friend class AudioManager;
private:
	static ResourceManager* manager;			///<单例指针
	static std::mutex m_Mutex;					///<锁

	ResourceManager() {};
	~ResourceManager() {};

	std::unordered_map<std::string, IMAGE*> ImagePool;		///<图片池	[图片名字,图像句柄]
	std::unordered_map<std::string, AnimationResource>  animationPool;			///<动画池	[动画名字，动画资源类]
	std::unordered_map<std::string,std::string>  AudioPool;			///<音频池		[音频名字，路径]
	
	///加载单张图片
	void Load(std::string name, std::string path, int wid, int hei);	
	///加载多张动画图片
	/// @remark 路径到文件 不用加结尾"/"
	void Load(std::string name, std::string path, int wid, int hei, int num);	
	/// 加载多张动画图片 
	/// @remark 每一张图片都要指定大小即长宽
	void Load(std::string name, std::string path, const std::vector<Pair>& arr);
	///加载Springsheet动画图片
	void Load(std::string name, std::string path, int wid, int hei, int num, int row, int col);		
	///加载音频媒体资源
	void Load(std::string name, std::string path);		
public:
	void Initialize();

	///获取实例
	static ResourceManager* instance();
	
	///根据名字返回图片池中相应图片指针
	IMAGE* Fetch(
		std::string name			///<[in]图片名字
	);		

	///根据名字返回动画池中相应动画资源
	AnimationResource AnimationFetch(
		std::string name			///<[in]动画名字
	);			

	///音频是否存在
	bool IsAudio(
		std::string name			///<[in]音频名字
	);
};