#pragma once
#pragma once
#include"BasePlant.h"

class SunFlower :public BasePlant
{
	
	class Animation idle;

	Timer ProduceTimerHandle;

	float transitionLevel = 0;		//��ʼû���˾��㼶
	int state = 0;		//ʲô��û�и�
public:
	SunFlower();

	virtual void Update(float deltaTime);

	void Produce();
};