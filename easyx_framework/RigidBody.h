/*****************************************************************//**
 * \file   RigidBody.h
 * \brief  刚体组件定义
 * @brief 简要模拟2D游戏场景中的物理运动规律
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"Component.h"

/**
 *  刚体组件.
 * @brief 利用物理 实现对物体坐标控制
 */
class RigidBody:public Component
{
	friend class Collider;

	Vector2D velocity = Vector2D(0, 0);		///<速度
	float maxSpeed = 1000.f;				///<最大速度
	float gravity = 9.8f;					///<重力加速度
	float angularVelocity = 0.f;			///<角速度 实现自旋的角度
	bool bGravityEnable = true;				///<是否实现重力计算
	bool bMoveable = true;					///<是否实现移动
	bool bRotatable = true;					///<是否实现旋转
	float mass = 1.0f;						///<质量

	float linearDrag = 0.f;					///<线性阻尼
	float angularDrag = 0.f;				///<角阻尼

	///每帧的时间
	float deltaTime = 0.f;

	///存储附着的碰撞体
	std::unordered_set <class Collider* > colliders;
	
	/**
	 * 物理模拟计算.
	 * 
	 * \param [in] impactnolunit 施加力的方向单位向量
	 * \param [in] another 碰撞到的物体刚体组件指针
	 */
	void RestrictVelocity(Vector2D impactnolunit, RigidBody* another = nullptr);
	
	/**
	 * 物理模拟计算.
	 * 
	 * \param [in] impactnolunit 施加力的方向单位向量
	 * \param [in] material 材质类
	 * \param [in] another 碰撞到的物体刚体组件指针
	 */
	void RestrictVelocity(Vector2D impactnolunit, const PhysicsMaterial& material, RigidBody* another = nullptr);

public:

	~RigidBody();

	virtual void  BeginPlay() {};

	virtual void Update(float deltaTime)override;

	///设置重力加速度
	void SetGravity(float gravity) { this->gravity = gravity; }

	///设置速度
	void SetVelocity(Vector2D velocity) { this->velocity = velocity; }

	///获取速度
	Vector2D GetVelocity() { return velocity; }

	///设置角阻尼
	void SetAngularVelocity(float angularVelocity) { this->angularVelocity = angularVelocity; }

	///添加持续力
	/// @remark 假设质量为1,牛二知道F=ma 持续力
	void AddForce(Vector2D force) { velocity += force * deltaTime /mass; }

	///添加冲量
	///@brief 瞬间添加一个力 瞬时力
	void AddPulse(Vector2D pulse) { velocity += pulse / mass; }						

	///是否可移动
	void SetMoveable(bool Moveable) { this->bMoveable = Moveable; if (!Moveable)SetVelocity(Vector2D(0, 0)); }

	///是否启用重力
	void SetGravityEnable(bool GravityEnable) { this->bGravityEnable = GravityEnable; }

	///是否可旋转
	void SetRotatable(bool Rotatable) { this->bRotatable = Rotatable; if (!Rotatable)angularVelocity = 0;}

	void SetMass(float mass) { this->mass = mass; }
	//设置速度最大值
	void SetMaxSpeed(float maxSpeed) { this->maxSpeed = maxSpeed; }

	//设置线性阻尼
	void SetLinearDrag(float linearDrag) { this->linearDrag = linearDrag; }

	//设置角阻尼
	void SetAngularDrag(float angularDrag) { this->angularDrag = angularDrag; }
};