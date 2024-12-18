/*****************************************************************//**
 * \file   GameStatics.h
 * \brief  全局静态库定义，提供处理游戏常见逻辑所需的接口，更方便访问并管理游戏对象
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"
#include<vector>
#include"UserInterface.h"
#include"Object.h"
#include"World.h"
/**
 * 全局静态库.
 */
class GameStatics final			
{
public:
	///获取游戏实例
	static GameInstance* GetGameInstance();

	///获取玩家控制器
	/// @remark 世界没有就会创建新的控制者
	static Controller* GetController();


	///创建物体实例对象,实现跟世界类的物体容器绑定
	/// @remark ！！！创建物体只能用这个创建
	/// @see Object
	template<class T>
	static T* CreateObject(Vector2D pos = Vector2D(0, 0)	///<[in]位置向量
	);

	///把world类物体容器中所有要找的类及其子类放进容器返回
	template<class T>
	static std::vector<T*> FindObjectsofClass();

	///创建UI对象
	///@remark ！！！创建UserInterface只能用这个创建
	/// @see UserInterface
	template<class T>
	static T* CreateUI();


	///加载地图
	static void OpenLevel(std::string levelName			///<[in]关卡类名字
	);

	///相机震动
	static void PlayCameraShake(
		int intensity			///<[in]震动强度
		, int decay = 20		///<[in]震动衰减系数 默认为20
	);

	///获取游戏持续时间
	static double GetTimeSeconds();

	///暂停游戏数秒
	static void Pause(float delay		///<[in]秒数
	);

	///播放音效片段
	static void PlaySound2D(
		std::string name		///<[in]音频名称
	);
};

template<class T>
inline T*  GameStatics::CreateObject(Vector2D pos )
{
	T* pObj = new T;
	if (pObj && static_cast<Object*>(pObj))		//用静态转化是因为子类转化为父类，不能父类转化成子类
	{
		mainWorld.GameObjects_to_add.push_back(pObj);
		pObj->SetLocalPosition(pos);
		return pObj;
	}
	return nullptr;
}

template<class T>
inline std::vector<T*> GameStatics::FindObjectsofClass()
{
	std::vector<T*> result;
	//预分配内存 ，防止在增删的时候频繁的申请内存 占用内存资源
	result.reserve(mainWorld.GameObjects.size());
	for (auto& obj : mainWorld.GameObjects)
	{
		if (T* pObj = static_cast<T*>(obj))result.push_back(pObj);
	}
	return result;
}

template<class T>
inline T* GameStatics::CreateUI()
{
	T* pUI = new T;
	if (pUI && static_cast<UserInterface*>(pUI))
	{
		mainWorld.GameUIs.insert(pUI);
		return pUI;
	}
	delete pUI;
	return nullptr;
}