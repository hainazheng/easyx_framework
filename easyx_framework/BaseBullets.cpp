#include "BaseBullets.h"
#include"Collider.h"
#include"BaseZomble.h"

BaseBullet::BaseBullet()
{
	renderer = GetComponentByClass<SpriteRenderer>();
	renderer->SetLayer(2);

	circle = ConstructComponent<CircleCollider>();
	circle->AttachTo(root);
	circle->SetRadius(12);
	circle->SetType(CollisionType::Bullet);
}

void BaseBullet::BeginPlay()
{
	Sprite::BeginPlay();
	circle->OnComponentBeginOverlap.Push(this,&BaseBullet::OnOverlap);
}

void BaseBullet::OnOverlap(Collider* overlapcomponentself, Collider* othercomponent, Object* otherobject)
{
	
	BaseZomble*  _basezomble=Cast<BaseZomble>(otherobject);
	if (!_basezomble)return;
	_basezomble->GetDamage(damage);
	Destroy();

}
