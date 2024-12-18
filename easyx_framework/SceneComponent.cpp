#include"SceneComponent.h"
#include"Object.h"

//使用递归来解除绑定并销毁 类似树的递归删除节点
void SceneComponent::process_Destruct()
{
	if (!children.empty())
	{
		for (auto& child : children)		//容器可以使用auto遍历
			child->process_Destruct();
	}
	Component::Destruct();
}


/**
 * 实现场景组件的绑定 同时实现物体和子组件的绑定.
 * 
 * \param 父节点
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
 * 解除场景组件之间的绑定.
 * 这里有个逻辑问题一定要先解除再绑定  
 * 解决办法1.加上判定条件if（parent==par)
 * \param 父节点
 */
void SceneComponent::DetachForm(SceneComponent* par)
{
	if (par)
	{
		par->children.erase(this);
		if (par == parent)parent = nullptr;
	}
}

///解除场景组件绑定并销毁
void SceneComponent::Destruct()
{
	if (parent != nullptr)
	{
		parent->children.erase(this);
	}
	process_Destruct();
}

///获取场景组件的世界坐标
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

///获取场景组件的世界旋转角度
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

///获取场景组件的世界的缩放大小
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
