#pragma once
#include"Sprite.h"
#include"Collider.h"
#include"Animator.h"

class BasePlant:public Sprite
{
protected:
	//需要的组件
	SpriteRenderer* renderer;
	BoxCollider* box;
	class Animator* ani;

	int blood = 10;
	int cost = 0;
public:
	BasePlant();
	void TakeDamage(float damage);
	virtual void BeginPlay()override;
	void Activate();
	int GetCost()const { return cost; }
};