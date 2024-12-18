#include"Component.h"
#include"Object.h"


//解除物体绑定组件，并销毁组件
void Component::Destruct()
{
	pOwner->UnregisterComponent(this);
	delete this;
}

void Component::Activate()
{
	bIsEnabled = true;
}

void Component::Deactivate()
{
	bIsEnabled = false;
}