#pragma once
#include"Sprite.h"
#include"Animator.h"
#include"Collider.h"
#include"RigidBody.h"
#include"Timer.h"
#include"Particle.h"

class plateform :public Sprite
{
	BoxCollider* box;
public:
	plateform()
	{
		box = ConstructComponent<BoxCollider>();
		box->AttachTo(root);
		box->SetSize({500,70});
		box->SetCollisionMode(ColliderMode::Collision);
		box->SetType(CollisionType::plateform);
		box->OnComponentBeginOverlap.Push([this](Collider* overlapcomp,Collider* overlapanothercomp, Object* overlapanotherator)
			{/*RigidBody* rigid = overlapanotherator->GetComponentByClass<RigidBody>(); if(rigid)rigid->SetMoveable(false);*/std::cout << "hallo haina"<<std::endl;  }
	     );
		box->OnComponentBeginOverlap.Push([this](Collider* overlapcomp, Collider* overlapanothercomp, Object* overlapanotherator)
			{RigidBody* rigid = overlapanotherator->GetComponentByClass<RigidBody>(); if (rigid)rigid->SetMoveable(false);  }
		);
		box->OnComponentHitOverlap.Push([this](Collider* overlapcomp, Collider* overlapanothercomp, Object* overlapanotherator, Vector2D impactnolunit, HitResult hitresult)
			{RigidBody* rigid = overlapanotherator->GetComponentByClass<RigidBody>(); /*if (rigid)rigid->SetGravityEnable(false);*//*if (rigid)rigid->SetGravityEnable(false);*/
		if(rigid)std::cout << rigid->GetVelocity()<<std::endl; }
		);
	/*	box->OnComponentBeginOverlap.Remove([this](Collider* overlapcomp, Collider* overlapanothercomp, Object* overlapanotherator)
			{RigidBody* rigid = overlapanotherator->GetComponentByClass<RigidBody>(); if (rigid)rigid->SetMoveable(false);  }
		);*/
	}
};