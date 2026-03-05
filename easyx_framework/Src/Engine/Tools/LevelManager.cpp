#include"ZHNpch.h"
#include "LevelManager.h"
#include"Menu.h"
#include"BattleLevel.h"
#include "DefaultLevel.h"

LevelManager* LevelManager::manager=nullptr;

std::mutex LevelManager::m_Mutex ;

LevelManager* LevelManager::instance()
{
	if (!manager)
	{
		std::unique_lock<std::mutex> lock(m_Mutex); // 加锁 一创建好对象就释放 耗时操作
		if (!manager)
		{
			manager = new LevelManager;
		}
	}
    return manager;
}


void LevelManager::Initialize()
{
	AddLevel<DefaultLevel>("default");
	AddLevel<BattleLevel>("DayLawn");
	SetDefaultLevel("DayLawn");
	AddLevel<Menu>("mainMenu"); /*RefreshLevel();*/
	
}

void LevelManager::SetDefaultLevel(std::string levelName)
{
	if (levelMap.find(levelName) == levelMap.end())return;
	level_to_delete = mainWorld.CurrentLevel;
	ready_level = levelMap[levelName];

}

void LevelManager::RefreshLevel()
{
	if (ready_level)
	{
		mainWorld.CurrentLevel = ready_level;
		ready_level =nullptr;
	}
	//只是交换场景，但是本身场景还是存在的
	if (level_to_delete)
	{
		/*level_to_delete->EndPlay();*/
		mainWorld.ClearWorld();
		/*delete level_to_delete;*/
		level_to_delete = nullptr;
		mainWorld.CurrentLevel->BeginPlay();
	}
}