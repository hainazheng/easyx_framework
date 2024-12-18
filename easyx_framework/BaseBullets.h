#pragma once
#include"Sprite.h"

//�ӵ�����
class BaseBullet:public Sprite
{
protected:
	//��Ҫ�����
	SpriteRenderer* renderer;
	class CircleCollider* circle;


	int damage = 1;
public:
	BaseBullet();
	virtual void BeginPlay();

	virtual void Update(float deltaTime) { Sprite::Update(deltaTime); };

	virtual void OnOverlap(Collider* overlapcomponentself, Collider* othercomponent, Object* otherobject);
};