#pragma once
#include"BaseBullets.h"

class Pea :public BaseBullet
{
public:
	Pea();

	void On_Update(float deltaTime);
};