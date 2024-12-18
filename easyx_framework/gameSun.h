#pragma once
#include"Sprite.h"
#include"Animator.h"


//Ñô¹â
class Sun : public Sprite
{
	SpriteRenderer* renderer;
	Animation ani;
	Animator* animator;
	class CircleCollider* collider;
	class RigidBody* rigidbody;

	int groundLine;		//地面水平线横坐标

	Timer DestroyTimerHandle;
	bool bFading = false;
	bool bGrowing = false;
	bool bPicking = false;
public:
	Sun();

	void Update(float deltaTime);

	void Fall(int groundLine);

	void Throw(int groundLine);
};