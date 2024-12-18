#include"FSM.h"
#include"Collider.h"
#include"Sprite.h"
#include"Animator.h"

class idlestatus :public FSMItems
{
public:
	idlestatus(int _status, FSMItems* _parent = nullptr, int _level = 0)
		:FSMItems(_status,parent,_level)
	{
		enter.Bind([this]() {ani->SetNode("sun"); ani->SetCalled(true); std::cout << "enter idle status" << std::endl; });
		act.Bind([this]() { if (fsm->GetOwner()->GetWorldPosition().y > 400) { fsm->TransitionState("white");
		}  });
		exit.Bind([this]() {ani->SetCalled(false); });
	}
};

class whitestatus :public FSMItems
{
public:
	whitestatus(int _status, FSMItems* _parent = nullptr, int _level = 0)
		:FSMItems(_status, parent, _level)
	{
		enter.Bind(
			[this]() {
			ani->SetNode("sun"); ani->SetCalled(true); /*std::cout << ;*/
			SpriteRenderer* renderer = fsm->GetOwner()->GetComponentByClass<SpriteRenderer>(); 
			renderer->AddFilter(true, RED, 100); std::cout << "enter white status" << std::endl;
			
			}
		);
		act.Bind([this]() {if (fsm->GetOwner()->GetWorldPosition().y <=400) {
			fsm->TransitionState("idle");
		}});
		exit.Bind([this]() {
		SpriteRenderer* renderer = fsm->GetOwner()->GetComponentByClass<SpriteRenderer>();
		renderer->RemoveFilter(1); ani->SetCalled(false); });
	}
};