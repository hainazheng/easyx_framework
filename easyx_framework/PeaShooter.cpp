#include "PeaShooter.h"
#include"GameStatics.h"
#include"pea.h"

PeaShooter::PeaShooter()
{
	idle.Load("peashooter");
	idle.SetInterval(0.07);


	ani->insert("idle", idle);
	ani->SetNode("idle");

	shootevent.Bind(this, &PeaShooter::Shoot);
	idle.AddNotification(11,shootevent);
	cost = 100;
}

void PeaShooter::Shoot()
{
	GameStatics::CreateObject<Pea>(GetWorldPosition() + Vector2D(20, -25));
}
