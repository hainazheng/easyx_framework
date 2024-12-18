#include "Animated.h"
#include "SpriteRenderer.h"
#include "Math.h"


Animated::Animated()
{
	render = ConstructComponent<SpriteRenderer>();
	/*SetRootComponent(render);*/
	render->AttachTo(root);
	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);
}

void Animated::Init(std::string name, float delay, int layer)
{
	idle.Load(name);
	idle.SetIndex(Math::RandInt(0, 5));
	idle.SetInterval(delay);

	ani->insert("idle", idle);
	ani->SetNode("idle");

	render->SetLayer(layer);
	render->SetLocalScale(Vector2D(1, 1) + Vector2D(0.05, 0) * layer);
}
