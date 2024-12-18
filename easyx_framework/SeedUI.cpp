#include "SeedUI.h"
#include "BattleController.h"
#include "GameStatics.h"

#include"PeaShooter.h"
#include"SunFlower.h"


std::string plantName[] = { "豌豆射手","向日葵","樱桃炸弹","坚果","土豆地雷","寒冰射手","大嘴花","双发射手" };
int plantPrice[] = { 100,50,150,50,25,175,150,200 };
float coolingTime[] = { 5,5,45,30,30,5,5,5 };

SeedUI::SeedUI()
{
	button = AddWidget<Button>();
	button->AttachTo(root);
	button->SetLayoutPattern(LayoutPattern::Center);
	/*button->SetPosPattern(LayoutPositionPattern::LeftTop);*/
	button->SetSize(root->GetSize());
	button->SetLayer(12);

	black = AddWidget<Image>();
	black->AttachTo(root);
	/*black->SetLayoutPattern(LayoutPattern::LeftTop);*/
	black->SetLayoutPattern(LayoutPattern::Center);
	//black->SetPosPattern(LayoutPositionPattern::LeftTop);
	black->SetSize(root->GetSize());
	black->SetLayer(13);
	black->LoadSprite("black");
	black->SetTransparency(125);
	

	gray = AddWidget<Image>();
	gray->AttachTo(root);
	/*gray->SetLayoutPattern(LayoutPattern::LeftMiddle);*/
	gray->SetLayoutPattern(LayoutPattern::Center);
	//gray->SetPosPattern(LayoutPositionPattern::LeftTop);
	gray->SetSize(root->GetSize());
	gray->SetLayer(13);
	gray->LoadSprite("gray");
	gray->SetTransparency(125);
	
}

void SeedUI::Init(int num)
{
	number = num;
	button->LoadNormalPicture("card" + std::to_string(number + 1));
	/*button->LoadNormalPicture("card1" );*/
	button->EnableInfoBox(true);
	button->OnMousePressedBegin.Push(this, &SeedUI::ReadyToPlant);
	
}

void SeedUI::Update(float deltaTime)
{
	UserInterface::Update(deltaTime);

	if (BattleController* pController = Cast<BattleController>(GameStatics::GetController()))
	{
		
		if (CoolingTimerHandle.GetDelay() < coolingTime[number])
		{
			button->SetInfoText("$9冷却中\n$4" + plantName[number]); /*std::cout << number;*/
		}
		else if (pController->GetSunshineNum() < plantPrice[number])
			button->SetInfoText("$9没有足够阳光\n$4" + plantName[number]);
		else
			button->SetInfoText(plantName[number]);


		if (pController->GetSunshineNum() < plantPrice[number] || CoolingTimerHandle.GetDelay() < coolingTime[number])
			gray->SetTransparency(125);
		else gray->SetTransparency(0);


		if (CoolingTimerHandle.GetDelay() < coolingTime[number])
		{
			black->SetStartAndEndLoc(Pair(0,0), Pair(int(black->GetSize().x),int(
				black->GetSize().y * (1 - CoolingTimerHandle.GetDelay() / coolingTime[number]))));/*std::cout << black->GetSize().y * (1 - CoolingTimerHandle.GetDelay() / coolingTime[number])<<std::endl;*/
		}
	}
}

void SeedUI::ReadyToPlant()
{
	if (BattleController* pController = Cast<BattleController>(GameStatics::GetController()))
	{
		if (CoolingTimerHandle.GetDelay() < coolingTime[number] || pController->GetSunshineNum() < plantPrice[number])return;

		switch (number)
		{
		case 0: pController->seedToPlant = GameStatics::CreateObject<PeaShooter>(); break;
		case 1: pController->seedToPlant = GameStatics::CreateObject<SunFlower>(); break;
		}

	
	}
}
