#include "BattleLevel.h"
#include"GameStatics.h"
#include"SpriteRenderer.h"
#include"sun.h"
#include"plateform.h"
#include"PeaShooter.h"
#include"NormalZomble.h"
#include"sunflower.h"
#include"BattleController.h"
#include"CoreMinimal.h"
#include"LevelManager.h"
#include "Menu.h"
#include"AudioManager.h"
#include"Animated.h"
#include"RainProducer.h"
#include"Perlin.h"

BattleLevel::BattleLevel()
{
	

	//background0->SetLocalPosition(Vector2D(300, 300));

	//plate = GameStatics::CreateObject<plateform>();
	//plate->SetLocalPosition(Vector2D(480, 500));

	//sunshine1 = GameStatics::CreateObject<sun>();
	///*sunshine->AttachTo(background0);*/
	//sunshine1->SetLocalPosition(Vector2D(300, 400));
	//std::cout << sunshine1->GetWorldPosition() << std::endl;

	//sunshine2 = GameStatics::CreateObject<sun>();
	//sunshine2->SetLocalPosition(Vector2D(310, 100));
	//std::cout << sunshine2->GetWorldPosition() << std::endl;
	/*Perlin p1;
	ImageToolkit::drawnoise(p1);*/

	

	/*sunshine->AddRotation(10.f);*/

	//entermenu.Bind(10, [this]() {/*LevelManager::instance()->AddLevel<Menu>("mainMenu");*/ LevelManager::instance()->SetDefaultLevel("mainMenu"); });
}

void BattleLevel::Update(float deltaTime)
{
	/*std::cout << background0->GetWorldPosition() << std::endl;*/

		/*sunshine->AddRotation(0.1f);*/
	
	/*std::cout << 1;*/
}

void BattleLevel::BeginPlay()
{
	mainController = GameStatics::CreateObject<BattleController>();
	/*mainController->SetRectFrame({0,0,1000,600});*/


	background0 = GameStatics::CreateObject<Sprite>();
	background0->GetComponentByClass<SpriteRenderer>()->LoadSprite("bg0");
	background0->GetComponentByClass<SpriteRenderer>()->SetLayer(-5);
	/*background0->GetComponentByClass<SpriteRenderer>()->MeanBlur(10);*/
	/*background0->GetComponentByClass<SpriteRenderer>()->WaveFrom(100,300,150,255);*/


	/*peashooter = GameStatics::CreateObject<PeaShooter>();
	peashooter->SetLocalPosition(Vector2D(310, 400));*/
	
	GameStatics::CreateObject<NormalZomble>(Vector2D(500, 400));
	

	//GameStatics::CreateObject<SunFlower>(Vector2D(310, 500));

	/*AudioManager::instance()->SetRandom(true);*/

	/*AudioManager::instance()->Play(std::string("secer"));*/
	/*AudioManager::instance()->NextSound();*/

	AudioManager::instance()->NextSound();

	//for (int i = 1; i < 8; i++) {
	//	Animated* rainland = GameStatics::CreateObject<Animated>({  float(i) * Math::RandInt(0,100),50 });
	//	rainland->Init("rain_land", 0.08f, 10);
	//	
	//}
	/*for (int i = 0; i < 10; i++) {
		Animated* rainland = GameStatics::CreateObject<Animated>({ -1500 + float(i) * Math::RandInt(100,500),-75 });
		rainland->Init("rain_land", 0.08f, 16); 
		rainland->SetLocalScale(Vector2D(1.25, 1.25) * Math::RandReal(0.8f, 1.2f));
	}*/
	/*AudioManager::instance()->Pause();*/
	/*GameStatics::CreateObject<RainProducer>({ 400,0 });*/
}
