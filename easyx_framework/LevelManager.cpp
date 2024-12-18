#include "LevelManager.h"
#include"Menu.h"
#include"BattleLevel.h"

LevelManager* LevelManager::manager=nullptr;

std::mutex LevelManager::m_Mutex ;

LevelManager* LevelManager::instance()
{
	if (!manager)
	{
		std::unique_lock<std::mutex> lock(m_Mutex); // ���� һ�����ö�����ͷ� ��ʱ����
		if (!manager)
		{
			manager = new LevelManager;
		}
	}
    return manager;
}


void LevelManager::Initialize()
{
	AddLevel<BattleLevel>("DayLawn");
	SetDefaultLevel("DayLawn");
	AddLevel<Menu>("mainMenu"); /*RefreshLevel();*/
}

void LevelManager::SetDefaultLevel(std::string levelName)
{
	level_to_delete = mainWorld.CurrentLevel;
	if (levelMap.find(levelName) == levelMap.end())return;
	mainWorld.CurrentLevel = levelMap[levelName];
}

void LevelManager::RefreshLevel()
{
	if (level_to_delete)
	{
		mainWorld.ClearWorld();
		level_to_delete = nullptr;
		mainWorld.CurrentLevel->BeginPlay();
	}
}