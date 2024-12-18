/*****************************************************************//**
 * \file   SceneComponent.h
 * \brief  场景组件类
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"Component.h"


/**
 * 场景组件基类.
 */
class SceneComponent :public Component
{
	/*friend class SpriteRenderer;*/

	Transform transform;				///<场景属性
	std::set<SceneComponent*> children;		///<绑定的孩子容器
protected:
	SceneComponent* parent = nullptr;			///<父指针

	/**
	 * 销毁组件
	 * @remark 使用递归来解除绑定并销毁 类似树的递归删除节点.
	 * 
	 */
	void process_Destruct();

public:

	virtual void Update(float deltaTime)override {};

	///设置所属组件
	void AttachTo(SceneComponent* par);

	/**
	 * 解除所属组件.
	 * @remark ！！！用户慎用
	 * \param [in] par父指针
	 */
	void DetachForm(SceneComponent* par);

	virtual void Destruct();

	virtual void BeginPlay()override {};

	virtual void EndPlay()override {};

	///获取相对坐标
	Vector2D GetLocalPosition() const { return transform.position; }			
	///获取相对角度
	float GetLocalRotation() const { return transform.rotation; }			
	///获取相对缩放大小
	Vector2D GetLocalScale() const { return transform.scale; }				
	Vector2D GetWorldPosition()const;										//获取绝对坐标
	float GetWorldRotation()const;											//获取绝对角度
	Vector2D GetWorldScale()const;											//获取绝对缩放大小
	///设置相对坐标
	void SetLocalPosition(const Vector2D& pos) { transform.position = pos; }			
	///设置相对角度
	void SetLocalRotation(float rot) { transform.rotation = rot; }					
	///设置相对缩放大小
	void SetLocalScale(const Vector2D& scale) { transform.scale = scale; }			
	///加上一个坐标
	void AddPosition(const Vector2D& pos) { transform.position += pos; }		
	///加上一个角度
	void AddRotation(float rot) { transform.rotation += rot; }					
};