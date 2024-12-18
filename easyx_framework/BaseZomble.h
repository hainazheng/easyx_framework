#pragma once
#include"Sprite.h"
#include"Collider.h"
#include"Animator.h"
#include"BasePlant.h"

class BaseZomble :public Sprite
{
	DECLARE_EDGE_CONDITION_BY_TYPE(FSMEdge, FSMItems)

protected:
	//需要的组件
	SpriteRenderer* renderer;
	BoxCollider* box;
	class Animator* ani;
	class FSM* fsm;			//状态机

	int blood = 5;
	float speed = 0.2f;

	Animation walk;
	Animation eat;
	Animation die;

	/*AnimationDelegate dieEvent;*/
	
	//存储重叠的植物物体
	//防止重叠突然变动来不及保存 也做了筛选
	std::set<Object*> OverlapPlants;

	BasePlant* eatingPlant=nullptr;

	/*FSMEdge walk_stand;*/
	CondiEdge<FSMItems> walk_stand;
	CondiEdge<FSMItems> stand_walk;

	bool bstand=false;
public:
	BaseZomble();

	virtual void BeginPlay();

	virtual void Update(float deltaTime)override;

	void GetDamage(float damage);

	void OnOverlap(Collider* overlapcomponentself, Collider* othercomponent, Object* otherobject);

	void EndOverlap(Collider* overlapcomponentself, Collider* othercomponent, Object* otherobject);
};