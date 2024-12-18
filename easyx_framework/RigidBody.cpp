#include "RigidBody.h"
#include"Math.h"
#include"Collider.h"

//����ģ�����
void RigidBody::RestrictVelocity(Vector2D impactnolunit, RigidBody* another)
{
	//������ʩ����������������
	Vector2D tangentVector = { impactnolunit.y, -impactnolunit.x };
	/*std::cout << 1;*/
	//�����ٶ�
	Vector2D normalVelocity = Vector2D::ProjectVector(velocity, impactnolunit);
	//�����ٶ�
	Vector2D tangentVelocity = Vector2D::ProjectVector(velocity, tangentVector);

	//Ħ���S��
	float friction = 0.2f;
	//���ԂS��  1Ϊ��ȫ������ײ
	float bounciness = 0.9f;

	//float friction = material.friction;
	//float bounciness = material.bounciness;

	//Ħ�����ı��ٶ�
	if (!another)
	{
		if (Vector2D::DotProduct(velocity, impactnolunit) < 0)
		{
			//�˔�
			float multiplier = (tangentVelocity.size() - normalVelocity.size() * friction) / tangentVelocity.size();
			multiplier = Math::Clamp(multiplier, 0.0f, 1.0f);
			velocity = tangentVelocity * multiplier - bounciness * normalVelocity;
		}
		return;
	}

	//��ײ�������巨���ٶ�
	Vector2D anotherNormalVelocity = Vector2D::ProjectVector(another->velocity, impactnolunit);
	//��ײ�������������ٶ�
	Vector2D anotherTangentVelocity = Vector2D::ProjectVector(another->velocity, tangentVector);

	/*std::cout <<normalVelocity-anotherNormalVelocity<<impactnolunit<< Vector2D::DotProduct(normalVelocity - anotherNormalVelocity, impactnolunit)<<std::endl;*/
	//std::cout /*<< normalVelocity - anotherNormalVelocity << impactnolunit */<<int( Vector2D::DotProduct(normalVelocity - anotherNormalVelocity, impactnolunit)) << std::endl;

	/* ������ײ�����߼� */
	if (Vector2D::DotProduct(normalVelocity - anotherNormalVelocity, impactnolunit) <= 0)return;//ȷ��������������

	//����֮ǰ�ķ����ٶ�
	Vector2D normalVelocity_ = normalVelocity;

	//������ײ֮��ķ�����ٶ�
	normalVelocity = ((mass - bounciness * another->mass) * normalVelocity + (1 + bounciness) * another->mass * anotherNormalVelocity) / (mass + another->mass);
	anotherNormalVelocity = ((another->mass - bounciness * mass) * anotherNormalVelocity + (1 + bounciness) * mass * normalVelocity_) / (mass + another->mass);

	velocity = normalVelocity + tangentVelocity;
	another->velocity = anotherNormalVelocity + anotherTangentVelocity;
	
}

void RigidBody::RestrictVelocity(Vector2D impactnolunit, const PhysicsMaterial& material, RigidBody* another)
{
	//������ʩ����������������
	Vector2D tangentVector = { impactnolunit.y, -impactnolunit.x };
	/*std::cout << 1;*/
	//�����ٶ�
	Vector2D normalVelocity = Vector2D::ProjectVector(velocity, impactnolunit);
	//�����ٶ�
	Vector2D tangentVelocity = Vector2D::ProjectVector(velocity, tangentVector);

	////Ħ���S��
	//float friction = 0.2f;
	////���ԂS��  1Ϊ��ȫ������ײ
	//float bounciness = 0.9f;

	float friction = material.friction;
	float bounciness = material.bounciness;

	//Ħ�����ı��ٶ�
	if (!another)
	{
		if (Vector2D::DotProduct(velocity, impactnolunit) < 0)
		{
			//�˔�
			float multiplier = (tangentVelocity.size() - normalVelocity.size() * friction) / tangentVelocity.size();
			multiplier = Math::Clamp(multiplier, 0.0f, 1.0f);
			velocity = tangentVelocity * multiplier - bounciness * normalVelocity;
		}
		return;
	}

	//��ײ�������巨���ٶ�
	Vector2D anotherNormalVelocity = Vector2D::ProjectVector(another->velocity, impactnolunit);
	//��ײ�������������ٶ�
	Vector2D anotherTangentVelocity = Vector2D::ProjectVector(another->velocity, tangentVector);

	/* ������ײ�����߼� */
	if (Vector2D::DotProduct(normalVelocity - anotherNormalVelocity, impactnolunit) <= 0)return;//ȷ��������������

	//����֮ǰ�ķ����ٶ�
	Vector2D normalVelocity_ = normalVelocity;

	//������ײ֮��ķ�����ٶ�
	normalVelocity = ((mass - bounciness * another->mass) * normalVelocity + (1 + bounciness) * another->mass * anotherNormalVelocity) / (mass + another->mass);
	anotherNormalVelocity = ((another->mass - bounciness * mass) * anotherNormalVelocity + (1 + bounciness) * mass * normalVelocity_) / (mass + another->mass);

	velocity = normalVelocity + tangentVelocity;
	another->velocity = anotherNormalVelocity + anotherTangentVelocity;
}

///��չ�������ײ�����ŵ������������
RigidBody::~RigidBody()
{
	//��չ�������ײ�����ŵ������������
	for (auto& collider : colliders)
	{
		collider->rigidAttached = nullptr;
	}
}

void RigidBody::Update(float deltaTime)
{
	this->deltaTime = deltaTime;
	Component::Update(deltaTime);
	if (!pOwner || !bIsEnabled)return;
	
		
	if (bMoveable)									//ʵ���ƶ�
	{
		if (bGravityEnable)
		{
			velocity.y += gravity * deltaTime;			//ÿһ֡˲ʱ�ٶȶ�Ҫ�����������ٶ����½��б�� Ч�����������������˶�
		}

		//ʵ����ײ֮����ٶȱ仯
		for (auto& me : colliders)
		{
			//ֻ���Լ��ͷ�����ײ�����嶼�ǿ���ײ�ĲŻᷢ����ײ
			if (me->GetCollisionMode() != ColliderMode::Collision)continue;

			for (auto& another : me->collisions)
			{
				if (another->GetCollisionMode() != ColliderMode::Collision)continue;
				/*RestrictVelocity(-me->CollisionHit(another).impactnormalunit, another->rigidAttached);*/
				RestrictVelocity(-me->CollisionHit(another).impactnormalunit,
					PhysicsMaterial::Combine(me->material, another->material), another->rigidAttached);
			}

		}
		//ʵ����ײ֮��������߽��������ײ�嵫��û����������֮��Ҫ����һ���谭�����˶�����
		for (auto& me : colliders)
		{
			if (me->GetCollisionMode() != ColliderMode::Collision)continue;

			for (auto& another : me->collisions)
			{
				//û�и��ŵ��������Ծ�û����ײ
				if (another->GetCollisionMode() != ColliderMode::Collision || another->rigidAttached)continue;
				/*RestrictVelocity(-me->CollisionHit(another).impactnormalunit);*/
				RestrictVelocity(-me->CollisionHit(another).impactnormalunit,
					PhysicsMaterial::Combine(me->material, another->material));

			}

		}

		pOwner->AddPosition(Vector2D(Math::Clamp(velocity.x, -maxSpeed, maxSpeed),
			Math::Clamp(velocity.y, -maxSpeed, maxSpeed)) * deltaTime);			//ÿ֡���ƶ��ľ��룬�ٶȵ�λ��ÿ��

		if (linearDrag) {
			float buffer = velocity.x - velocity.x * linearDrag * deltaTime / mass;
			velocity.x = ((*(int*)&velocity.x) ^ (*(int*)&buffer)) < 0 ? 0 : buffer;
			buffer = velocity.y - velocity.y * linearDrag * deltaTime / mass;
			velocity.y = ((*(int*)&velocity.y) ^ (*(int*)&buffer)) < 0 ? 0 : buffer;
		}
	}


		if (bRotatable)									//��������
		{
			pOwner->AddRotation(angularVelocity* deltaTime);
			if (angularDrag) {
				float buffer = angularVelocity - angularVelocity * angularDrag * deltaTime / mass;
				angularVelocity = ((*(int*)&angularVelocity) ^ (*(int*)&buffer)) < 0 ? 0 : buffer;
			}
		}
	
}
