/*****************************************************************//**
 * \file   enemy.h
 * \brief  创建敌人类
 * 
 * \author zhn
 * \date   January 2025
 *********************************************************************/
#pragma once
#include"Sprite.h"
#include"Animator.h"


/**
 * 敌人基类.
 * @brief 实现基本敌人逻辑
 */
class Enemy :public Sprite
{
public:
	Enemy();

	virtual void BeginPlay()override;

	virtual void On_Update(float deltaTime)override;

	//void TakeDamage(Vector2D normal, bool gain = true);

	//bool IsDead()const { return bIsDead; }

protected:
	//void OnOverlap(class Collider* hitComp, Collider* otherComp, Object* otherActor);

	//virtual void Die();

	class SpriteRenderer* render;
	class Animation enemy1;
	//class SpriteRenderer* render_death;
	//class CircleCollider* circle;
	//class RigidBody* rigid;
	//class AudioPlayer* audio;
	class Animator* ani;



	//int blood = 10;
	//bool bIsDead = false;
};
