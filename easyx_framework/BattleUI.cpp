#include "BattleUI.h"
#include "GameStatics.h"
#include "BattleController.h"
#include "SeedUI.h"

BattleUI::BattleUI()
{
	SeedBank = AddWidget<Image>();
	SeedBank->AttachTo(root);
	SeedBank->SetLayoutPattern(LayoutPattern::MiddleTop);
	/*SeedBank->SetPosPattern(LayoutPositionPattern::LeftTop);*/
	SeedBank->SetRelativePosition(Vector2D(-130, 45.5));
	SeedBank->SetSize(Vector2D(530, 91));
	SeedBank->LoadSprite("seedbank");
	SeedBank->SetLayer(10);

	Panel = AddWidget<HorizontalPanel>();
	Panel->AttachTo(SeedBank); std::cout << "seedbank" <<SeedBank->GetScreenPosition() << std::endl;
	Panel->SetLayoutPattern(LayoutPattern::Center);
	//Panel->SetPosPattern(LayoutPositionPattern::LeftTop);
	Panel->SetRelativePosition(Vector2D(33, 0));
	
	Panel->SetUnitSize(Vector2D(54, 77));
	Panel->SetSpacing(2);
	Panel->SetLayer(11);
	

	SunShine = AddWidget<Text>();
	SunShine->AttachTo(SeedBank);
	SunShine->SetRelativePosition(Vector2D(38, 77));
	SunShine->SetLayer(11);

	for (int i = 0; i < 8; i++)
	{
		SeedUI* ui = GameStatics::CreateUI<SeedUI>();
		ui->Init(i);
		Panel->AddMember(ui);
		
	}
}

void BattleUI::Update(float deltaTime)
{
	UserInterface::Update(deltaTime);
	if (BattleController* pController = Cast<BattleController>(GameStatics::GetController()))
	{
		SunShine->SetText(std::to_string(pController->GetSunshineNum()), 4);
	}

}
