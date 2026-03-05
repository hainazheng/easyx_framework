#pragma once
#include"BasePlant.h"

class PeaShooter:public BasePlant
{
	AnimationDelegate shootevent;
	class Animation idle;	
public:
	PeaShooter();
	void Shoot();
};