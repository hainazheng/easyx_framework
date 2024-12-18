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

///设置虚函数，把不能写在析构却需要优先实现的写在这
/// 例如delete this
void Object::EndPlay()
{
	for (auto& it : components)
	{

		it->EndPlay();
	}
}

//实现物体之间的绑定
void Object::AttachTo(Object* par)
{
	if (par)
	{
		par->children.insert(this);
		this->parent = par;

	}
}

//解除物体之间的绑定
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

///用深度优先遍历进行遍历，借助栈来实现，实现物体及其所有绑定的物体类销毁
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

///递归获取物体类的世界坐标
Vector2D Object::GetWorldPosition() const
{

	if (parent != nullptr) { return (parent->GetWorldPosition() + GetLocalPosition()); }
	else
	{
		return GetLocalPosition();
	}
}

///递归获取物体类的世界旋转角度
float Object::GetWorldRotation() const
{
	if (parent != nullptr)return parent->GetWorldRotation() + GetLocalRotation();
	else
	{
		return GetLocalRotation();
	}
}

///递归获取物体类的世界缩放大小
Vector2D Object::GetWorldScale() const
{
	if (parent != nullptr)return parent->GetWorldScale() * GetLocalScale();
	else
	{
		return GetLocalScale();
	}
}

///调试 在画布上显示物体的坐标
void Object::DrawDebugPosition() const
{
	Vector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetCameraPosition())
		* (20.f / mainWorld.mainCamera->GetSpringArmLength()) + Vector2D(getwidth() / 2, getheight() / 2);
	outtextxy((int)pos.x, (int)pos.y,
		LPCTSTR(GetWorldPosition().To_String().c_str()));
}
