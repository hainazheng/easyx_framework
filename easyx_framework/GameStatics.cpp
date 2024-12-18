#include "GameStatics.h"
#include"Controller.h"
#include"LevelManager.h"
#include"Camera.h"
#include"Timer.h"
extern bool bFrameLimitOn = true;

extern int MaxFrame = 100;

GameInstance* GameStatics::GetGameInstance()
{
    //µ¥Àý¶ÔÏó
    if (!mainWorld.gameInstance)mainWorld.gameInstance = new GameInstance;
        return mainWorld.gameInstance;
}

Controller* GameStatics::GetController()
{
    Controller* pController = mainWorld.CurrentLevel->mainController;
    /*Controller* pController = mainWorld.mainController;*/
    if (!pController)pController = CreateObject<Controller>();
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
