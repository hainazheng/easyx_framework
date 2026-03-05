#include "ZHNpch.h"
#include "pea.h"

Pea::Pea()
{
	renderer->LoadSprite("pea");
}

void Pea::On_Update(float deltaTime)
{
	BaseBullet::On_Update(deltaTime);
	AddPosition({ 3,0 });
	if (GetWorldPosition().x > 1000)Destroy();
}
