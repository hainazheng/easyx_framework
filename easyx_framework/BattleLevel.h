#pragma once
#include"CoreMinimal.h"
#include"Sprite.h"
#include"Animator.h"
#include"Collider.h"
#include"RigidBody.h"
#include"Timer.h"


class BattleLevel:public Level
{
	//需要渲染所以创建一个sprite对象
	class Sprite* background0;
	/*class sun* sunshine1;
	class sun* sunshine2;
	class plateform* plate;*/
	class PeaShooter* peashooter;
	//Timer entermenu;
	/*Timer trans;*/
public:
	BattleLevel();
	virtual void Update(float deltaTime);
	virtual void BeginPlay();
};