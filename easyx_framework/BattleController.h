#pragma once
#include"Controller.h"
#include"Timer.h"

class BattleController:public Controller
{


	friend class SeedUI;

	Timer OverallTimeHandle;
	Timer ShakeTimeHandle;

	Timer SunshineTimerHandle;

	int sunshine = 100;

	class BasePlant* seedToPlant = nullptr;


public:
	void Update(float deltaTime);
	void SetupInputComponent(InputComponent* inputcomponent);
	
	BattleController();
	void shake();			//Υπ¶―
	void MoveRight();
	void MoveLeft();
	void MoveDown();
	void Jump();
	void ZoomIn();
	void ZoomOut();

	int GetSunshineNum() const { return sunshine; }

	void SetSunshineNum(int num) { sunshine = num; }

	void PlantSeed();

	void ProduceSunshine();

	//void UserClickMove(SpriteRenderer* a);
};