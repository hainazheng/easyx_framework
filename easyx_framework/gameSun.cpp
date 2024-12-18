#include "gameSun.h"
#include"Collider.h"
#include"RigidBody.h"
#include"Math.h"
#include"GameStatics.h"
#include"BattleController.h"

Sun::Sun()
{
	renderer = GetComponentByClass<SpriteRenderer>();
	renderer->SetLayer(2);
	animator = ConstructComponent<Animator>();
	ani.Load("sunshine");
	ani.SetInterval(0.08f);

	animator->insert("idle", ani);
	animator->SetNode("idle");

	collider = ConstructComponent<CircleCollider>();
	collider->AttachTo(root);
	collider->SetCollisionMode(ColliderMode::Trigger);
	collider->SetRadius(35);

	rigidbody = ConstructComponent<RigidBody>();
	rigidbody->SetGravity(400.f);

	groundLine = Math::RandInt(100, 500);

	DestroyTimerHandle.Bind(10, [this]() {bFading = true; });
}


void Sun::Update(float deltaTime)
{
	Sprite::Update(deltaTime);

	if (GetWorldPosition().y > groundLine && !bPicking)
	{
		rigidbody->SetGravityEnable(false);
		rigidbody->SetMoveable(false);
	}

	if (bFading)
	{
		BYTE tran = renderer->GetTransparency() - 5;
		renderer->SetTransparency(tran);
		if (tran <= 5)Destroy();
	}

	if (bGrowing)
	{
		Vector2D scale = GetLocalScale() + Vector2D(1, 1) * 0.05f;
		if (scale.x >= 1.f) { bGrowing = false; scale = { 1,1 }; }
		SetLocalScale(scale);
	}

	if (bPicking)
	{
		if (BattleController* pController = Cast<BattleController>(GameStatics::GetController()))
		{
			if (GetWorldPosition().y > 25)return;
			pController->SetSunshineNum(pController->GetSunshineNum() + 25);
			Destroy();
		}
		return;
	}

	//ÅÐ¶ÏÊ°È¡
	if (Controller* pController = GameStatics::GetController())
	{
		if (collider->IsMouseOver() && pController->IsMouseClick())
		{
			bPicking = true;
			bFading = false;
			rigidbody->SetGravity(false);
			rigidbody->SetMoveable(true);
			/*rigidbody->SetVelocity((-GetWorldPosition() + Vector2D(25, 25)) * 1.25);*/

			rigidbody->SetVelocity((-GetWorldPosition() + GameStatics::GetController()-> GetWorldPosition() +Vector2D(25, 25)) * 1.25);
		}
	}
}


void Sun::Fall(int groundLine)
{
	rigidbody->SetGravityEnable(false);
	this->groundLine = groundLine;
	rigidbody->SetVelocity(Vector2D(0, 50));
	DestroyTimerHandle.SetDelay(15);
}

void Sun::Throw(int groundLine)
{
	rigidbody->AddPulse(Vector2D(Math::RandInt(-40, 50), -125));
	this->groundLine = groundLine;
	bGrowing = true;
	SetLocalScale(Vector2D(1, 1) * 0.1f);
}