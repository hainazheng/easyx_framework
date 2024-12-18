#include "Menu.h"
#include"GameStatics.h"
#include"SpriteRenderer.h"
#include"sun.h"
#include"plateform.h"
#include"BattleController.h"

Menu::Menu()
{

}

void Menu::BeginPlay()
{
	Level::BeginPlay();
	mainController = GameStatics::CreateObject<BattleController>();
	background0 = GameStatics::CreateObject<Sprite>();
	background0->GetComponentByClass<SpriteRenderer>()->LoadSprite("bg0");
	background0->GetComponentByClass<SpriteRenderer>()->SetLayer(-5);

	plate = GameStatics::CreateObject<plateform>();
	plate->SetLocalPosition(Vector2D(480, 500));

	sunshine1 = GameStatics::CreateObject<sun>();
	/*sunshine->AttachTo(background0);*/
	sunshine1->SetLocalPosition(Vector2D(300, 400));
	std::cout << sunshine1->GetWorldPosition() << std::endl;

	sunshine2 = GameStatics::CreateObject<sun>();
	sunshine2->SetLocalPosition(Vector2D(310, 100));
	std::cout << sunshine2->GetWorldPosition() << std::endl;

}

void Menu::Update(float deltaTime)
{

}
