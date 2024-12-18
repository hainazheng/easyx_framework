#pragma once
#include"Sprite.h"

//子弹基类
class BaseBullet:public Sprite
{
protected:
	//需要的组件
	SpriteRenderer* renderer;
	class CircleCollider* circle;


	int damage = 1;
public:
	BaseBullet();
	virtual void BeginPlay();

	virtual void Update(float deltaTime) { Sprite::Update(deltaTime); };

	virtual void OnOverlap(Collider* overlapcomponentself, Collider* othercomponent, Object* otherobject);
};