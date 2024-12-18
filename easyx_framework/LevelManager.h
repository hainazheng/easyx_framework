/*****************************************************************//**
 * \file   LevelManager.h
 * \brief  关卡管理器
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <string>
#include<mutex>
#include<thread>


class Level;

/**
 * 关卡管理类 
 * 单例的懒汉模式.
 * @see Level
 */
class LevelManager
{
	friend class Level;
	friend class World;
public:

	static LevelManager* instance();

	///初始化关卡
	void Initialize();

	///设置默认关卡
	void SetDefaultLevel(std::string levelName			///<[in]关卡名字
	);

	///更新关卡
	void RefreshLevel();
	
	///创建并添加关卡
	/// @remark ！！！只能用这个创建新关卡
	template<class T>
	void AddLevel(std::string levelName///<[in]关卡名字
	);

private:

	static LevelManager* manager;

	static std::mutex m_Mutex;

	LevelManager() {};

	~LevelManager() {};

	std::unordered_map<std::string, Level*>levelMap;		///<[关卡名字，关卡类]

	

	Level* level_to_delete = nullptr;						///准备删除的关卡
};

//创建关卡
template<class T>
inline void LevelManager::AddLevel(std::string levelName)
{
	T* pLevel = new T;
	if (pLevel && static_cast<Level*>(pLevel))
	{
		levelMap.insert({ levelName,pLevel });
		return;
	}
	delete pLevel;
}

