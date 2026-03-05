#pragma once
#include "UserInterface.h"
#include "Timer.h"


class SeedUI :public UserInterface
{
	Button* button;

	Image* gray;

	Image* black;

	int number = 0;

	Timer CoolingTimerHandle;
public:
	SeedUI();

	void Init(int num);

	virtual void On_Update(float deltaTime)override;

	void ReadyToPlant();
};