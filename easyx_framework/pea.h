#pragma once
#include"BaseBullets.h"

class Pea :public BaseBullet
{
public:
	Pea();

	void Update(float deltaTime);
};