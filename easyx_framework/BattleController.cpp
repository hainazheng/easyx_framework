#include "BattleController.h"
#include"Camera.h"
#include"GameStatics.h"
#include"gameSun.h"
#include"BattleUI.h"
#include"BasePlant.h"
#include"SeedUI.h"
#include <graphics.h>
//#include"SpriteRenderer.h"
void BattleController::Update(float deltaTime)
{
	Controller::Update(deltaTime);
	////改变控制者的绝对坐标
	//if (OverallTimeHandle.GetDelay() > 2.f && OverallTimeHandle.GetDelay() < 5.f && GetWorldPosition().x <= 600)
	//{
	//	AddPosition(Vector2D(3.f,0));
	//	Camera* _camera = GetComponentByClass<Camera>();
	//	/*std::cout <<  _camera->GetCameraPosition() << std::endl;
	//	std::cout << GetWorldPosition() << std::endl;*/
	//	/*std::cout << _camera->GetCameraPosition() - GetWorldPosition() << std::endl;*/
	//}
	//if (OverallTimeHandle.GetDelay() > 7.f && GetWorldPosition().x >= 225.f)
	//{
	//	AddPosition(Vector2D(-3.f, 0));
	//	Camera* _camera = GetComponentByClass<Camera>();
	//	/*std::cout << _camera->GetCameraPosition() << std::endl;
	//	std::cout << GetWorldPosition() << std::endl;*/
	//	/*std::cout << _camera->GetCameraPosition()-GetWorldPosition() << std::endl;*/
	//}
	if (seedToPlant)
	{
	Collider* col=seedToPlant->GetComponentByClass<Collider>();
	if (col)col->SetType(CollisionType::Default);
		seedToPlant->SetLocalPosition(GetMousePosition());

	}

}

void BattleController::SetupInputComponent(InputComponent* inputcomponent)
{
	Controller::SetupInputComponent(inputcomponent);		//这个inputcomponent就是我们需要的基类的私有成员
	inputcomponent->SetMapping("MoveRight", VK_D);
	inputcomponent->BindAction("MoveRight", InputType::Holding, this, &BattleController::MoveRight);
	inputcomponent->SetMapping("MoveLeft", VK_A);
	inputcomponent->BindAction("MoveLeft", InputType::Holding, this, &BattleController::MoveLeft);
	inputcomponent->SetMapping("MoveDown", VK_S);
	inputcomponent->BindAction("MoveDown", InputType::Holding, this, &BattleController::MoveDown);
	inputcomponent->SetMapping("Jump", VK_J);
	inputcomponent->BindAction("Jump", InputType::Pressed, this, &BattleController::Jump);

	inputcomponent->SetMapping("ZoomIn", VK_Q);
	inputcomponent->BindAction("ZoomIn", InputType::Holding, this, &BattleController::ZoomIn);
	inputcomponent->SetMapping("ZoomOut", VK_E);
	inputcomponent->BindAction("ZoomOut", InputType::Holding, this, &BattleController::ZoomOut);

	inputcomponent->SetMapping("Plant", KeyCode::VK_LButton);
	inputcomponent->BindAction("Plant", InputType::Pressed, this, &BattleController::PlantSeed);


}

BattleController::BattleController()
{
	ShakeTimeHandle.Bind(3.f, this, &BattleController::shake,false);
	ShakeTimeHandle.Stop();

	BattleUI* ui = GameStatics::CreateUI<BattleUI>();
	ui->AddToViewport();


	SunshineTimerHandle.Bind(12, this, &BattleController::ProduceSunshine, true, 7.5);
}

//实现震动
void BattleController::shake()
{
	
	camera->ShakeCamera(20.f,10);
}

void BattleController::MoveLeft()
{
	AddPosition(Vector2D(-0.5f, 0));
}

void BattleController::MoveDown()
{
	AddPosition(Vector2D(0, 0.5f));
}

void BattleController::Jump()
{
	AddPosition(Vector2D(0,-100.f));
}

//int count=0;
void BattleController::MoveRight()
{
	AddPosition(Vector2D(0.5f, 0));
	//std::cout << count++ << std::endl;
}

//放大图像
void BattleController::ZoomIn()
{
	camera->SetSpringArmLength(armLength -= 0.05f);
}

//缩小图像
void BattleController::ZoomOut()
{
	camera->SetSpringArmLength(armLength += 0.05f);
}

void BattleController::PlantSeed()
{
	if (seedToPlant)
	{
		Collider* col = seedToPlant->GetComponentByClass<Collider>();
		if (col)col->SetType(CollisionType::plant);
		seedToPlant->Activate();
		sunshine -= seedToPlant->GetCost();
		seedToPlant = nullptr;
	}
}

void BattleController::ProduceSunshine()
{
	Sun* sunshine = GameStatics::CreateObject<Sun>(Vector2D(Math::RandInt(100, 700), 0));
	if (sunshine)sunshine->Fall(Math::RandInt(250, 550));
}

//void BattleController::UserClickMove(SpriteRenderer* a)
//{
//	Vector2D pos = GetMousePosition();
//	ExMessage msg;
//
//	while (peekmessage(&msg, EM_MOUSE))
//	{
//		if (msg.message == WM_MOUSEMOVE)
//		{
//			a-> WaveFrom(pos.x, pos.y, 15, 2560);
//		}
//		else if (msg.message == WM_LBUTTONDOWN)
//		{
//			a->WaveFrom(pos.x, pos.y, 30, 2560);
//		}
//	}
//
//	
//}