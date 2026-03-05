#pragma once
#include "CoreMinimal.h"
#include "Animator.h"
#include"Object.h"


class Animated :public Object
{
public:
	Animated();

	void Init(std::string name, float delay, int layer);

protected:
	class SpriteRenderer* render;
	Animator* ani;

	Animation idle;
};
