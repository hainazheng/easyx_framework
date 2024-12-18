#pragma once
#include"Sprite.h"
#include"Collider.h"
#include"Animator.h"
#include"BasePlant.h"

class BaseZomble :public Sprite
{
	DECLARE_EDGE_CONDITION_BY_TYPE(FSMEdge, FSMItems)

protected:
	//��Ҫ�����
	SpriteRenderer* renderer;
	BoxCollider* box;
	class Animator* ani;
	class FSM* fsm;			//״̬��

	int blood = 5;
	float speed = 0.2f;

	Animation walk;
	Animation eat;
	Animation die;

	/*AnimationDelegate dieEvent;*/
	
	//�洢�ص���ֲ������
	//��ֹ�ص�ͻȻ�䶯���������� Ҳ����ɸѡ
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