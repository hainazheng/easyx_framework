#include"Component.h"
#include"Object.h"


//����������������������
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