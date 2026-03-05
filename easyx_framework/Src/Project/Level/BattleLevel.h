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
	class Sprite* background0 = nullptr;
	/*class sun* sunshine1;
	class sun* sunshine2;
	class plateform* plate;*/
	class PeaShooter* peashooter = nullptr;
	class Particle_ember* ember=nullptr;
	class SunFlower* sunflower = nullptr;
	/*Timer test;*/
	Timer entermenu;
	/*Timer trans;*/
public:
	BattleLevel();
	virtual void On_Update(float deltaTime);
	virtual void BeginPlay();
};