/*****************************************************************//**
 * \file   RigidBody.h
 * \brief  �����������
 * @brief ��Ҫģ��2D��Ϸ�����е������˶�����
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"Component.h"

/**
 *  �������.
 * @brief �������� ʵ�ֶ������������
 */
class RigidBody:public Component
{
	friend class Collider;

	Vector2D velocity = Vector2D(0, 0);		///<�ٶ�
	float maxSpeed = 1000.f;				///<����ٶ�
	float gravity = 9.8f;					///<�������ٶ�
	float angularVelocity = 0.f;			///<���ٶ� ʵ�������ĽǶ�
	bool bGravityEnable = true;				///<�Ƿ�ʵ����������
	bool bMoveable = true;					///<�Ƿ�ʵ���ƶ�
	bool bRotatable = true;					///<�Ƿ�ʵ����ת
	float mass = 1.0f;						///<����

	float linearDrag = 0.f;					///<��������
	float angularDrag = 0.f;				///<������

	///ÿ֡��ʱ��
	float deltaTime = 0.f;

	///�洢���ŵ���ײ��
	std::unordered_set <class Collider* > colliders;
	
	/**
	 * ����ģ�����.
	 * 
	 * \param [in] impactnolunit ʩ�����ķ���λ����
	 * \param [in] another ��ײ��������������ָ��
	 */
	void RestrictVelocity(Vector2D impactnolunit, RigidBody* another = nullptr);
	
	/**
	 * ����ģ�����.
	 * 
	 * \param [in] impactnolunit ʩ�����ķ���λ����
	 * \param [in] material ������
	 * \param [in] another ��ײ��������������ָ��
	 */
	void RestrictVelocity(Vector2D impactnolunit, const PhysicsMaterial& material, RigidBody* another = nullptr);

public:

	~RigidBody();

	virtual void  BeginPlay() {};

	virtual void Update(float deltaTime)override;

	///�����������ٶ�
	void SetGravity(float gravity) { this->gravity = gravity; }

	///�����ٶ�
	void SetVelocity(Vector2D velocity) { this->velocity = velocity; }

	///��ȡ�ٶ�
	Vector2D GetVelocity() { return velocity; }

	///���ý�����
	void SetAngularVelocity(float angularVelocity) { this->angularVelocity = angularVelocity; }

	///��ӳ�����
	/// @remark ��������Ϊ1,ţ��֪��F=ma ������
	void AddForce(Vector2D force) { velocity += force * deltaTime /mass; }

	///��ӳ���
	///@brief ˲�����һ���� ˲ʱ��
	void AddPulse(Vector2D pulse) { velocity += pulse / mass; }						

	///�Ƿ���ƶ�
	void SetMoveable(bool Moveable) { this->bMoveable = Moveable; if (!Moveable)SetVelocity(Vector2D(0, 0)); }

	///�Ƿ���������
	void SetGravityEnable(bool GravityEnable) { this->bGravityEnable = GravityEnable; }

	///�Ƿ����ת
	void SetRotatable(bool Rotatable) { this->bRotatable = Rotatable; if (!Rotatable)angularVelocity = 0;}

	void SetMass(float mass) { this->mass = mass; }
	//�����ٶ����ֵ
	void SetMaxSpeed(float maxSpeed) { this->maxSpeed = maxSpeed; }

	//������������
	void SetLinearDrag(float linearDrag) { this->linearDrag = linearDrag; }

	//���ý�����
	void SetAngularDrag(float angularDrag) { this->angularDrag = angularDrag; }
};