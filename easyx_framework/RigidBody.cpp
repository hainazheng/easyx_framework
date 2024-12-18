#include "RigidBody.h"
#include"Math.h"
#include"Collider.h"

//物理模拟计算
void RigidBody::RestrictVelocity(Vector2D impactnolunit, RigidBody* another)
{
	//对物体施加力切向向量方向
	Vector2D tangentVector = { impactnolunit.y, -impactnolunit.x };
	/*std::cout << 1;*/
	//法向速度
	Vector2D normalVelocity = Vector2D::ProjectVector(velocity, impactnolunit);
	//切向速度
	Vector2D tangentVelocity = Vector2D::ProjectVector(velocity, tangentVector);

	//摩擦係數
	float friction = 0.2f;
	//彈性係數  1为完全弹性碰撞
	float bounciness = 0.9f;

	//float friction = material.friction;
	//float bounciness = material.bounciness;

	//摩擦力改变速度
	if (!another)
	{
		if (Vector2D::DotProduct(velocity, impactnolunit) < 0)
		{
			//乘數
			float multiplier = (tangentVelocity.size() - normalVelocity.size() * friction) / tangentVelocity.size();
			multiplier = Math::Clamp(multiplier, 0.0f, 1.0f);
			velocity = tangentVelocity * multiplier - bounciness * normalVelocity;
		}
		return;
	}

	//碰撞到的物体法向速度
	Vector2D anotherNormalVelocity = Vector2D::ProjectVector(another->velocity, impactnolunit);
	//碰撞到的物体切向速度
	Vector2D anotherTangentVelocity = Vector2D::ProjectVector(another->velocity, tangentVector);

	/*std::cout <<normalVelocity-anotherNormalVelocity<<impactnolunit<< Vector2D::DotProduct(normalVelocity - anotherNormalVelocity, impactnolunit)<<std::endl;*/
	//std::cout /*<< normalVelocity - anotherNormalVelocity << impactnolunit */<<int( Vector2D::DotProduct(normalVelocity - anotherNormalVelocity, impactnolunit)) << std::endl;

	/* 弹性碰撞处理逻辑 */
	if (Vector2D::DotProduct(normalVelocity - anotherNormalVelocity, impactnolunit) <= 0)return;//确保有相碰的趋势

	//保留之前的法向速度
	Vector2D normalVelocity_ = normalVelocity;

	//计算碰撞之后的法向的速度
	normalVelocity = ((mass - bounciness * another->mass) * normalVelocity + (1 + bounciness) * another->mass * anotherNormalVelocity) / (mass + another->mass);
	anotherNormalVelocity = ((another->mass - bounciness * mass) * anotherNormalVelocity + (1 + bounciness) * mass * normalVelocity_) / (mass + another->mass);

	velocity = normalVelocity + tangentVelocity;
	another->velocity = anotherNormalVelocity + anotherTangentVelocity;
	
}

void RigidBody::RestrictVelocity(Vector2D impactnolunit, const PhysicsMaterial& material, RigidBody* another)
{
	//对物体施加力切向向量方向
	Vector2D tangentVector = { impactnolunit.y, -impactnolunit.x };
	/*std::cout << 1;*/
	//法向速度
	Vector2D normalVelocity = Vector2D::ProjectVector(velocity, impactnolunit);
	//切向速度
	Vector2D tangentVelocity = Vector2D::ProjectVector(velocity, tangentVector);

	////摩擦係數
	//float friction = 0.2f;
	////彈性係數  1为完全弹性碰撞
	//float bounciness = 0.9f;

	float friction = material.friction;
	float bounciness = material.bounciness;

	//摩擦力改变速度
	if (!another)
	{
		if (Vector2D::DotProduct(velocity, impactnolunit) < 0)
		{
			//乘數
			float multiplier = (tangentVelocity.size() - normalVelocity.size() * friction) / tangentVelocity.size();
			multiplier = Math::Clamp(multiplier, 0.0f, 1.0f);
			velocity = tangentVelocity * multiplier - bounciness * normalVelocity;
		}
		return;
	}

	//碰撞到的物体法向速度
	Vector2D anotherNormalVelocity = Vector2D::ProjectVector(another->velocity, impactnolunit);
	//碰撞到的物体切向速度
	Vector2D anotherTangentVelocity = Vector2D::ProjectVector(another->velocity, tangentVector);

	/* 弹性碰撞处理逻辑 */
	if (Vector2D::DotProduct(normalVelocity - anotherNormalVelocity, impactnolunit) <= 0)return;//确保有相碰的趋势

	//保留之前的法向速度
	Vector2D normalVelocity_ = normalVelocity;

	//计算碰撞之后的法向的速度
	normalVelocity = ((mass - bounciness * another->mass) * normalVelocity + (1 + bounciness) * another->mass * anotherNormalVelocity) / (mass + another->mass);
	anotherNormalVelocity = ((another->mass - bounciness * mass) * anotherNormalVelocity + (1 + bounciness) * mass * normalVelocity_) / (mass + another->mass);

	velocity = normalVelocity + tangentVelocity;
	another->velocity = anotherNormalVelocity + anotherTangentVelocity;
}

///清空关联的碰撞器附着的这个物理属性
RigidBody::~RigidBody()
{
	//清空关联的碰撞器附着的这个物理属性
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
	
		
	if (bMoveable)									//实现移动
	{
		if (bGravityEnable)
		{
			velocity.y += gravity * deltaTime;			//每一帧瞬时速度都要随着重力加速度往下进行变大 效果就是向下做加速运动
		}

		//实现碰撞之后的速度变化
		for (auto& me : colliders)
		{
			//只有自己和发生碰撞的物体都是可碰撞的才会发生碰撞
			if (me->GetCollisionMode() != ColliderMode::Collision)continue;

			for (auto& another : me->collisions)
			{
				if (another->GetCollisionMode() != ColliderMode::Collision)continue;
				/*RestrictVelocity(-me->CollisionHit(another).impactnormalunit, another->rigidAttached);*/
				RestrictVelocity(-me->CollisionHit(another).impactnormalunit,
					PhysicsMaterial::Combine(me->material, another->material), another->rigidAttached);
			}

		}
		//实现碰撞之后的碰到边界或者是碰撞体但是没有物理属性之后要产生一个阻碍物体运动的力
		for (auto& me : colliders)
		{
			if (me->GetCollisionMode() != ColliderMode::Collision)continue;

			for (auto& another : me->collisions)
			{
				//没有附着的物理属性就没有碰撞
				if (another->GetCollisionMode() != ColliderMode::Collision || another->rigidAttached)continue;
				/*RestrictVelocity(-me->CollisionHit(another).impactnormalunit);*/
				RestrictVelocity(-me->CollisionHit(another).impactnormalunit,
					PhysicsMaterial::Combine(me->material, another->material));

			}

		}

		pOwner->AddPosition(Vector2D(Math::Clamp(velocity.x, -maxSpeed, maxSpeed),
			Math::Clamp(velocity.y, -maxSpeed, maxSpeed)) * deltaTime);			//每帧的移动的距离，速度单位是每秒

		if (linearDrag) {
			float buffer = velocity.x - velocity.x * linearDrag * deltaTime / mass;
			velocity.x = ((*(int*)&velocity.x) ^ (*(int*)&buffer)) < 0 ? 0 : buffer;
			buffer = velocity.y - velocity.y * linearDrag * deltaTime / mass;
			velocity.y = ((*(int*)&velocity.y) ^ (*(int*)&buffer)) < 0 ? 0 : buffer;
		}
	}


		if (bRotatable)									//进行自旋
		{
			pOwner->AddRotation(angularVelocity* deltaTime);
			if (angularDrag) {
				float buffer = angularVelocity - angularVelocity * angularDrag * deltaTime / mass;
				angularVelocity = ((*(int*)&angularVelocity) ^ (*(int*)&buffer)) < 0 ? 0 : buffer;
			}
		}
	
}
