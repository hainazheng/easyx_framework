#include "pea.h"

Pea::Pea()
{
	renderer->LoadSprite("pea");
}

void Pea::Update(float deltaTime)
{
	BaseBullet::Update(deltaTime);
	AddPosition({ 3,0 });
	if (GetWorldPosition().x > 1000)Destroy();
}
