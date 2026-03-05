#pragma once
#include"CoreMinimal.h"

class DefaultLevel :public Level
{

public:
	DefaultLevel();
	virtual void On_Update(float deltaTime);
	virtual void BeginPlay();
};