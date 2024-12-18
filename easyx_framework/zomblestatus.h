#pragma once
#include"FSM.h"
#include"Collider.h"
#include"Sprite.h"
//#include"AudioManager.h"

class walkstatus :public FSMItems
{
public:
	walkstatus(int _status, FSMItems* _parent = nullptr, int _level = 0)
		:FSMItems(_status, _parent, _level)
	{
		
		enter.Bind([this]() {
			ani->SetNode("walk"); ani->SetCalled(true);
			std::cout << "enter walk status" << std::endl;
			
			//col->OnComponentBeginOverlap.Bind([this](Collider* overlapcomponentself, Collider* othercomponent, Object* otherobject) 
			//	{fsm->TransitionState("stand"); });
			});
		act.Bind([this]() {
			/*fsm->GetOwner()->AddPosition({ -fsm->GetOwner()->speed, 0 });*/
		  });
		exit.Bind([this]() { ani->SetCalled(false);

		//col->OnComponentBeginOverlap.Remove([this](Collider* overlapcomponentself, Collider* othercomponent, Object* otherobject)
		//	{fsm->TransitionState("stand"); });
			});
	}
};

class standstatus :public FSMItems
{
public:
	standstatus(int _status, FSMItems* _parent = nullptr, int _level = 0)
		:FSMItems(_status, _parent, _level)
	{
		enter.Bind(
			[this]() {
				if (fsm->GetLastStatus() == std::string("walk"))ani->SetNode("eat"); std::cout << "enter stand status" << std::endl;
				ani->SetCalled(true);
			}
		);
		act.Bind([this]() {/*if (fsm->GetOwner()->GetWorldPosition().y <= 400) {
			fsm->TransitionState("idle");
		}*/});
		exit.Bind([this]() {
			/*SpriteRenderer* renderer = fsm->GetOwner()->GetComponentByClass<SpriteRenderer>();
			renderer->RemoveFilter(1);*/ ani->SetCalled(false); });
	}
};