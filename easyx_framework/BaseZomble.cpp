#include "BaseZomble.h"
#include"FSM.h"
#include"zomblestatus.h"


BaseZomble::BaseZomble()
{
	renderer = GetComponentByClass<SpriteRenderer>();
	renderer->SetLayer(1);
	renderer->EnableFilter();

	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetSize({ 40, 100 });
	box->SetType(CollisionType::Zomble);



	ani = ConstructComponent<Animator>();


	die.Load("zombledie");
	die.SetInterval(0.12f);
	ani->insert("die",die);

	fsm = ConstructComponent<FSM>();

	fsm->AddStatus<walkstatus>("walk");
	fsm->AddStatus<standstatus>("stand");
	
	fsm->AddParamater("wts", ParamType::Bool);
	
	walk_stand.Init(fsm->FindStatusByName("walk"),fsm->FindStatusByName("stand"));
	walk_stand.AddCondition(TransitionCondition::Bool{ "wts" ,true});

	stand_walk.Init(fsm->FindStatusByName("stand"), fsm->FindStatusByName("walk"));
	stand_walk.AddCondition(TransitionCondition::Bool{ "wts" ,false });

	/*fsm->TransitionState("walk");*/
	/*dieEvent.Push([this]() {this->Destroy(); });

	die.AddNotification(8, dieEvent);*/

	
}

void BaseZomble::BeginPlay()
{
	Sprite::BeginPlay();

	box->OnComponentBeginOverlap.Push(this, &BaseZomble::OnOverlap);
	box->OnComponentEndOverlap.Push(this, &BaseZomble::EndOverlap);
	
}

void BaseZomble::Update(float deltaTime)
{
	Sprite::Update(deltaTime);
	if (blood > 0 && fsm->getNameStatus(fsm->GetNormalStatus())=="walk")AddPosition({-speed,0});
	if (GetWorldPosition().x < 0)Destroy();
}

void BaseZomble::GetDamage(float damage)
{
	blood -= damage;
	Blink(0.2f, WHITE);

	if (blood <= 0)
	{
		ani->SetNode("die");
	}
}

void BaseZomble::OnOverlap(Collider* overlapcomponentself, Collider* othercomponent, Object* otherobject)
{
	//拿到植物的物体
	if (othercomponent->GetType() != CollisionType::plant)return;
	
	OverlapPlants.insert(otherobject);
	if (fsm->getNameStatus(fsm->GetNormalStatus()) == "walk")
	{
		bstand = true;
		/*fsm->TransitionState("stand");*/
		fsm->SetValue("wts",bstand);

		eatingPlant = Cast<BasePlant>(otherobject);

	}
}

void BaseZomble::EndOverlap(Collider* overlapcomponentself, Collider* othercomponent, Object* otherobject)
{

	if (othercomponent->GetType() != CollisionType::plant)return;
	//删去碰撞结束的植物
	OverlapPlants.erase(otherobject);
	
	if (OverlapPlants.empty())
	{
		bstand = false; fsm->SetValue("wts", bstand);
		/*fsm->TransitionState("walk");*/
		eatingPlant = nullptr;
	}
	else
	{
		eatingPlant = Cast<BasePlant>(*OverlapPlants.begin());
	}
}
