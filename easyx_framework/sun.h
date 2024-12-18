#pragma once
#include"Sprite.h"
#include"Animator.h"
#include"Collider.h"
#include"RigidBody.h"
#include"Timer.h"
#include"Particle.h"
#include"FSM.h"
#include"NewStatus.h"

//创建阳光物体
class sun:public Sprite
{
	Animation ani;
	Animator* animator;
	CircleCollider* circle;
	/*BoxCollider* box;*/
	RigidBody* rigidbody;
	/*Timer clock;*/
	/*Timer Blinkclock;*/
	Particle *particle;
	FSM *fsm;
public:
	/*void filtershake()
	{
		SpriteRenderer* renderer = StaticMesh::GetComponentByClass<SpriteRenderer>();
		renderer->EnableFilter();
	}*/
	/*void MyBlink()
	{
		SpriteRenderer* renderer = Sprite::GetComponentByClass<SpriteRenderer>();
		renderer->AddFilter(true, RED, 50);
		renderer->EnableFilLayer(1);
		Blink(0.5f, WHITE, 45);
		
		
		renderer->EnableFilter();
		
	}*/
	sun()
	{
		/*clock.Bind(1, this, &sun::filtershake, true);*/
		SpriteRenderer* renderer = GetComponentByClass<SpriteRenderer>();

		
		renderer->SetLayer(10);
		renderer->SetFilter(true,BLUE,50);
		/*renderer->AddFilter(true, RED, 100);*/
		/*renderer->RemoveFilter(1);*/
		renderer->EnableFilter();
		

		animator = ConstructComponent<Animator>();
		fsm = ConstructComponent<FSM>();
		
		/*ani.Load("normalwalk");*/
		ani.Load("sunshine");
		ani.SetInterval(0.07f);

		animator->insert("sun", ani);
		/*animator->SetNode("sun");*/
		/*animator->SetCalled(false);*/

		fsm->AddStatus<idlestatus>("idle");
		fsm->AddStatus<whitestatus>("white");
		fsm->TransitionState("idle");
		/*fsm->TransitionState("root");*/

		circle = ConstructComponent<CircleCollider>();
		circle->AttachTo(root);
		circle->SetRadius(35);
		circle->SetCollisionMode(ColliderMode::Collision);
		circle->SetType(CollisionType::sunshine);

	/*	box = ConstructComponent<BoxCollider>();
		box->AttachTo(root);
		box->SetSize({70,70});*/

		rigidbody = ConstructComponent<RigidBody>();
		/*rigidbody->SetMass(10.f);*/
		/*rigidbody->SetMoveable(true);
		rigidbody->SetGravity(1);*/
		/*rigidbody->SetForce({ 100,-200 });*/
		/*Blinkclock.Bind(5, this, &sun::MyBlink, true);*/
		/*rigidbody->SetGravity(100);*/

		particle = Object::ConstructComponent<Particle>();
		particle->  AttachTo(root);
		particle->Load("mineparticle");
		particle->SetCapacity(10);
		particle->SetPattern((ParticlePattern::Center));
		/*particle->SetSpeed(100.f);*/
		particle->SetLayer(10);
		particle->SetLifetime(2.f);
		particle->SetInterval(0);
		/*particle->SetCenter();*/
	}

	virtual void Update(float deltaTime)override
	{
		Sprite::Update(deltaTime);
		
		/*static Vector2D scale = { 1,1 };
		SetLocalScale(scale *= 1.005);*/
	}
	
};