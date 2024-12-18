#include "NormalZomble.h"
#include"zomblestatus.h"
#include"AudioManager.h"

NormalZomble::NormalZomble()
{
	walk.Load("normalwalk");
	walk.SetInterval(0.06f);
	eat.Load("normaleat");
	eat.SetInterval(0.06f);
	ani->insert("walk", walk);
	ani->insert("eat", eat);
	ani->SetNode("walk");
	
	AnimationDelegate dieEvent;

	dieEvent.Bind([this]() {  this->Destroy(); });
	
	die.AddNotification(0, dieEvent);
	eatEvent.Bind([this]() {if (eatingPlant)eatingPlant->TakeDamage(1.f); });
	eat.AddNotification(10, eatEvent);
	eat.AddNotification(30, eatEvent);

	fsm->TransitionState("walk");
}
