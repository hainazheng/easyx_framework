#pragma once
#include"CoreMinimal.h"
#include"Sprite.h"
#include"Animator.h"
#include"Collider.h"
#include"RigidBody.h"
#include"Timer.h"

class Menu :public Level
{
	//需要渲染所以创建一个sprite对象
	class Sprite* background0;
	class sun* sunshine1;
	class sun* sunshine2;
	class plateform* plate;


public:
	Menu();
	virtual void BeginPlay();
	virtual void Update(float deltaTime);
};