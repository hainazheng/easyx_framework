#include "ZHNpch.h"
#include "enemy.h"


Enemy::Enemy()
{
	render = GetComponentByClass<SpriteRenderer>();
	render->SetLayer(1);
	render->EnableFilter();
	enemy1.Load("enemy1");
	enemy1.SetInterval(0.12);
	ani=ConstructComponent<Animator>();
	
}

void Enemy::BeginPlay()
{
	Sprite::BeginPlay();


	ani->insert("enemy1",enemy1);
	ani->SetNode("enemy1");
	
	/*ani->Activate();*/
}

void Enemy::On_Update(float deltaTime)
{
	Sprite::On_Update(deltaTime);
}
