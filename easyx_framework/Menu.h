#pragma once
#include"CoreMinimal.h"
#include"Sprite.h"
#include"Animator.h"
#include"Collider.h"
#include"RigidBody.h"
#include"Timer.h"

class Menu :public Level
{
	//��Ҫ��Ⱦ���Դ���һ��sprite����
	class Sprite* background0;
	class sun* sunshine1;
	class sun* sunshine2;
	class plateform* plate;


public:
	Menu();
	virtual void BeginPlay();
	virtual void Update(float deltaTime);
};