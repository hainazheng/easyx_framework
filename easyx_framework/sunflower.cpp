#include "sunflower.h"
#include"GameStatics.h"
#include"gameSun.h"

SunFlower::SunFlower()
{
	idle.Load("sunflower");
	idle.SetInterval(0.06);

	ani->insert("idle", idle);
	ani->SetNode("idle");

	ProduceTimerHandle.Bind(20, [this]() {state = 1; }, true, 7.5);
	cost = 50;
}

void SunFlower::Update(float deltaTime)
{
	BasePlant::Update(deltaTime);

	if (state == 1)			//逐渐变深，准备生产
	{
		renderer->SetFilter(true, RGB(255, 245, 140), transitionLevel += 0.4f);
		if (transitionLevel >= 45)
		{
			Produce();
			state = 2;
		}
	}
	else if (state == 2)			//逐渐变回去
	{
		renderer->SetFilter(true, RGB(255, 245, 140), transitionLevel -= 0.3f);
		if (transitionLevel <= 1)
		{
			state = 0; transitionLevel = 0;
			renderer->SetFilter(false);
		}
	}
}

void SunFlower::Produce()
{
	Sun* sunshine = GameStatics::CreateObject<Sun>(GetWorldPosition() - Vector2D(10, 20));
	if (sunshine)sunshine->Throw(GetWorldPosition().y + 25);
}

