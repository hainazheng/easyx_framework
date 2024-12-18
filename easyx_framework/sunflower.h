#pragma once
#pragma once
#include"BasePlant.h"

class SunFlower :public BasePlant
{
	
	class Animation idle;

	Timer ProduceTimerHandle;

	float transitionLevel = 0;		//开始没有滤镜层级
	int state = 0;		//什么都没有干
public:
	SunFlower();

	virtual void Update(float deltaTime);

	void Produce();
};