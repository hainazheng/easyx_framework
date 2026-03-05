#include"ZHNpch.h"
#include "GameStatics.h"
#include"Controller.h"
#include"LevelManager.h"
#include"Camera.h"
#include"Timer.h"

GameInstance* GameStatics::GetGameInstance()
{
    //单例对象
    if (!mainWorld.gameInstance)mainWorld.gameInstance = new GameInstance;
        return mainWorld.gameInstance;
}

Controller* GameStatics::GetController()
{
    //问题:Controller是一个object，清空世界之后会造成空指针
    if (!mainWorld.CurrentLevel)
    {
        return nullptr;
    }
    Controller* pController = mainWorld.CurrentLevel->mainController;
    /*Controller* pController = mainWorld.mainController;*/
    if (!pController)
    {
        pController = CreateObject<Controller>(); mainWorld.CurrentLevel->mainController = pController;
    }
    return pController;
}

void GameStatics::OpenLevel(std::string levelName)
{
    mainWorld.levelManager->SetDefaultLevel(levelName);
}

void GameStatics::PlayCameraShake(int intensity, int decay)
{
    mainWorld.mainCamera->ShakeCamera(intensity, decay);
}

double GameStatics::GetTimeSeconds()
{
    return mainWorld.OverallClock->GetDelay();
}

void GameStatics::Pause(float delay)
{
    Sleep(uint32(1000 * delay));
    mainWorld.deltaTime -= delay;
}

void GameStatics::PlaySound2D(std::string name)
{
    mciSendString((std::string("seek ") + name + std::string(" to start")).c_str(), 0, 0, 0);
    mciSendString((std::string("play ") + name).c_str(), 0, 0, 0);
}
