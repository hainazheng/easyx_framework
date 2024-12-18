/*****************************************************************//**
 * \file   Component.h
 * \brief  组件基类
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"

/**
 * 组件基类.
 */
class Component :public base
{

protected:
	class Object* pOwner = nullptr;		///<处理控制逻辑  有对象物体的指针才能进行控制

	bool bIsEnabled = true;				///<是否激活
public:
	virtual void Update(float deltaTime)override {};

	/**
	 * 设置纯虚函数，把不能写在构造却需要优先实现的写在这.
	 */
	virtual void BeginPlay() {};

	///把一些不能写在析构函数的一些操作写在这里
	virtual void EndPlay() {};

	/**
	 * 设置绑定对象.
	 * 
	 * @param 绑定对象
	 */
	void SetOwner(class Object* owner) { pOwner = owner; }

	///解除物体绑定组件，并销毁组件
	virtual void Destruct();

	///激活组件
	virtual void Activate();

	///停用组件
	virtual void Deactivate();

	///获取绑定对象指针
	Object* GetOwner() const
	{
		return pOwner;
	}
};