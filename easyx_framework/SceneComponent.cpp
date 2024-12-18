#include"SceneComponent.h"
#include"Object.h"

//ʹ�õݹ�������󶨲����� �������ĵݹ�ɾ���ڵ�
void SceneComponent::process_Destruct()
{
	if (!children.empty())
	{
		for (auto& child : children)		//��������ʹ��auto����
			child->process_Destruct();
	}
	Component::Destruct();
}


/**
 * ʵ�ֳ�������İ� ͬʱʵ�������������İ�.
 * 
 * \param ���ڵ�
 */
void SceneComponent::AttachTo(SceneComponent* par)
{
	if (par)
	{
		par->children.insert(this);
		parent = par;
		SetOwner(par->pOwner);
	}
}

/**
 * ����������֮��İ�.
 * �����и��߼�����һ��Ҫ�Ƚ���ٰ�  
 * ����취1.�����ж�����if��parent==par)
 * \param ���ڵ�
 */
void SceneComponent::DetachForm(SceneComponent* par)
{
	if (par)
	{
		par->children.erase(this);
		if (par == parent)parent = nullptr;
	}
}

///�����������󶨲�����
void SceneComponent::Destruct()
{
	if (parent != nullptr)
	{
		parent->children.erase(this);
	}
	process_Destruct();
}

///��ȡ�����������������
Vector2D SceneComponent::GetWorldPosition() const
{
	if (parent != nullptr)return parent->GetWorldPosition() + GetLocalPosition();
	else {
		if (pOwner != nullptr)
		{
			return pOwner->GetWorldPosition();
		}
		else
		{
			return GetLocalPosition();
		}
	}
}

///��ȡ���������������ת�Ƕ�
float SceneComponent::GetWorldRotation() const
{
	if (parent != nullptr)return parent->GetWorldRotation() + GetLocalRotation();
	else {
		if (pOwner != nullptr)
		{
			return pOwner->GetWorldRotation();
		}
		else
		{
			return GetLocalRotation();
		}
	}
}

///��ȡ�����������������Ŵ�С
Vector2D SceneComponent::GetWorldScale() const
{
	if (parent != nullptr)return parent->GetWorldScale() * GetLocalScale();
	else {
		if (pOwner != nullptr)
		{
			return pOwner->GetWorldScale();
		}
		else
		{
			return GetLocalScale();
		}
	}
}
