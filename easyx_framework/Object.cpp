#include"SceneComponent.h"
#include"Object.h"
#include<stack>
#include"Camera.h"
#include"easyx.h"
#include"World.h"

void Object::BeginPlay()
{
	for (auto& it : components)
	{

		it->BeginPlay();
	}
}

///�����麯�����Ѳ���д������ȴ��Ҫ����ʵ�ֵ�д����
/// ����delete this
void Object::EndPlay()
{
	for (auto& it : components)
	{

		it->EndPlay();
	}
}

//ʵ������֮��İ�
void Object::AttachTo(Object* par)
{
	if (par)
	{
		par->children.insert(this);
		this->parent = par;

	}
}

//�������֮��İ�
void Object::DetachForm(Object* par)
{
	if (par) {
		par->children.erase(this);
		if (par == parent)parent = nullptr;
	}
}

void Object::SetRootComponent(SceneComponent* newroot)
{

	const_cast<SceneComponent*&>(root) = newroot;

}

///��������ȱ������б���������ջ��ʵ�֣�ʵ�����弰�����а󶨵�����������
void Object::Destroy()
{
	if (parent)parent->children.erase(this);

	std::stack<Object*>objects_to_delete;

	objects_to_delete.push(this);
	/*this->EndPlay();*/
	while (!objects_to_delete.empty())
	{
		Object* current_object = objects_to_delete.top();
		objects_to_delete.pop();

		if (!current_object->children.empty())
		{
			for (auto& child : current_object->children)
			{
				objects_to_delete.push(child);
				/*child->EndPlay();*/
			}

		}
		mainWorld.GameObjects_to_delete.insert(current_object);

	}
}

///�ݹ��ȡ���������������
Vector2D Object::GetWorldPosition() const
{

	if (parent != nullptr) { return (parent->GetWorldPosition() + GetLocalPosition()); }
	else
	{
		return GetLocalPosition();
	}
}

///�ݹ��ȡ�������������ת�Ƕ�
float Object::GetWorldRotation() const
{
	if (parent != nullptr)return parent->GetWorldRotation() + GetLocalRotation();
	else
	{
		return GetLocalRotation();
	}
}

///�ݹ��ȡ��������������Ŵ�С
Vector2D Object::GetWorldScale() const
{
	if (parent != nullptr)return parent->GetWorldScale() * GetLocalScale();
	else
	{
		return GetLocalScale();
	}
}

///���� �ڻ�������ʾ���������
void Object::DrawDebugPosition() const
{
	Vector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetCameraPosition())
		* (20.f / mainWorld.mainCamera->GetSpringArmLength()) + Vector2D(getwidth() / 2, getheight() / 2);
	outtextxy((int)pos.x, (int)pos.y,
		LPCTSTR(GetWorldPosition().To_String().c_str()));
}
