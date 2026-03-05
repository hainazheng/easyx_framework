#include"ZHNpch.h"
#include "Character.h"
#include"SpriteRenderer.h"
#include"RigidBody.h"
#include"Collider.h"
#include"Camera.h"

Character::Character()
{
	movementState = CharacterMovementState::Standing;
	bAddMoving = false;
	MoveFlag = 0;

	render = ConstructComponent<SpriteRenderer>();
	box = ConstructComponent<BoxCollider>();
	rigid = ConstructComponent<RigidBody>();

	SetRootComponent(render);
	Camera* camera = GetComponentByClass<Camera>();
	camera->AttachTo(root);
	box->AttachTo(root);
}

void Character::BeginPlay()
{
	Controller::BeginPlay();

	box->OnComponentHitOverlap.Push(this, &Character::OnLanded);
}

void Character::On_Update(float deltaTime)
{
	Controller::On_Update(deltaTime);

	if (box->IsCollisionsEmpty())
	{
		movementState = CharacterMovementState::Flying;
	}

	if (MoveFlag > -1)
	{
		MoveFlag--;
	}

	if (MoveFlag == 0 && movementState != CharacterMovementState::Standing)
	{
		Vector2D vel = rigid->GetVelocity(); vel.x = 0;
		rigid->SetVelocity(vel);
		if (movementState != CharacterMovementState::Flying)movementState = CharacterMovementState::Standing;
	}
}

void Character::AddInputX(float inputValue, bool bControlScale)
{
	if (bControlScale)SetLocalScale({ (inputValue >= 0.f ? 1.f : -1.f),1.f });
	MoveFlag = Key_Response;	//影响移动响应即时性跟帧数有关
	if (movementState == CharacterMovementState::Standing)
		movementState = CharacterMovementState::Running;
	if (std::abs(rigid->GetVelocity().x) > 150 * std::abs(inputValue))
	{
		rigid->AddPulse({ rigid->GetVelocity().x / std::abs(rigid->GetVelocity().x) * (-1.25f) * std::abs(inputValue),0 }); return;
	}
	rigid->AddPulse({ inputValue,0 });
}



void Character::OnLanded(Collider* hitComp, Collider* otherComp, Object* otherActor, Vector2D normalImpulse, const HitResult& hitResult)
{
	if (normalImpulse.y < 0 && movementState == CharacterMovementState::Flying)
	{
		movementState = CharacterMovementState::Standing;
	}
}