/*****************************************************************//**
 * \file   Collider.h
 * \brief  ��Ϸ������������ײ����¼��Ĵ���
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"
#include<vector>
#include"ColliderManager.h"
#include<functional>
#include"Delegate.h"
#include"RigidBody.h"
#include"SceneComponent.h"
#include"World.h"

/**
 * ��ײ��״����.
 * @remark ����Ժ�������չ���࣬Ҫ�ں������
 */
enum class ColliderShape :uint8{
Circle			///<Բ����ײ
, Box			///<������ײ
};

/**
 * ��ײģʽ.
 * @brief ��ײ����ģʽ���������ͬ��Ч��
 */
enum class ColliderMode:uint8{
	None		///<û��ģʽ���൱��û�������ײ�������������ײ�ж�
	,Trigger	///<����ģʽ,ֻ�ᷢ����ײȻ�����¼�
	,Collision	///<��ײģʽ���൱������ʵ����������ײ
};


/**
 * ��ײ�����
 * @remark ����������Ҫ����Ϣ.
 */
struct  HitResult
{
	Vector2D impactpoint;			///<�е�
	Vector2D impactnormalunit;		///<������������λ���� ����ײ��������
	Object* hitobject;				///<���е�����
	Component* hitcomponent;		///<���е����

	HitResult() :impactpoint(0, 0), impactnormalunit(0, 0), hitobject(nullptr), hitcomponent(nullptr) {};
	HitResult(const Vector2D& point, const Vector2D& tangnormal, Object* object, Component* component) :
		impactpoint(point), impactnormalunit(tangnormal), hitobject(object), hitcomponent(component) {};
};

class Collider;

/**
 * ��ײ�ص��¼�ί��.
 * @remark �����[Collider* overlapcomponentself,Collider* othercomponent,Object* otherobject]
 */
DECLARE_MULTI_PARAM_MULTI_DELEGATE_CLASS(CollisionOverlapDelegate, Collider*, Collider*, Object*)

/**
 * ����ģ��������ײ�¼�ί��.
 * �����[Collider* overlapcomponentself,Collider* othercomponent,Object* otherobject,Vector2D normalpurse ���������,const HitResult& hitresult]
 */
DECLARE_MULTI_PARAM_MULTI_DELEGATE_CLASS(CollisionHitOverlapDelegate, Collider*, Collider*, Object*,Vector2D,const HitResult&)



/**
 * ��ײί��(��һ�汾��.
 * @brief ��д ������ײ��ʱ��ִ�����ί�к������õ�����������
 * �õ�����ִ�п���Ȩ
 * @see MultiCastDelegate
 */
class CollisionDelegate
{
	std::vector <std::function<void(Collider*,Object*)>>callbacks;		///��װ����ص����� ��ø�����ײ����ײ��������

public:
	///��Ӻ���
	template<typename T>
	void Add(T* obj, void(T::* func)(Collider*, Object*))
	{
		callbacks.push_back(std::bind(func, obj, std::placeholders::_1, std::placeholders::_2));
	}
	///��Ӻ���
	void Add(std::function<void(Collider*, Object*)>func)
	{
		callbacks.push_back(func);
	}

	///�Ƴ���Ӧί��
	template<typename T>
	void Remove(T* obj, void(T::* func)(Collider*, Object*))
	{
		std::function<void(Collider*, Object*)>Targetfunc = std::bind(func,obj,std::placeholders::_1,std::placeholders::_2);
		for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
		{
			if (it->target<void(Collider*, Object*)>() == Targetfunc.target<void(Collider*, Object*)>())
			{
				callbacks.erase(it); break;
			}
		}
	}

	///�Ƴ���Ӧί��
	void Remove(std::function<void(Collider*, Object*)>func)
	{
		for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
		{
			if (it->target<void(Collider*, Object*)>() == func.target<void(Collider*, Object*)>())
			{
				callbacks.erase(it); break;
			}
		}
	}

	///����ί��(ִ������ί��)
	void BroadCast(Collider* OverlapCollider,Object* OverlapActor);

};


/**
 * ��ײ������.
 * @brief ʵ����ײ.ʵ��һЩ��ײ���л����߼��Ĵ�����������ڵĹ���
 * @remark �µ���ײ�����Ͷ�Ҫ�̳���
 */
class Collider :public SceneComponent
{
	friend class Controller;
	friend class RigidBody;
	friend void World::ProcessColliderZones();
	/*friend class World;*/
	
public:

	/**
	 * ���캯��.
	 * ������ײ�����в���
	 */
	Collider() :type(CollisionType::Default),mode(ColliderMode::Trigger){ mainWorld.GameColliders.insert(this); }
	virtual ~Collider() { /*if (rigidAttached)rigidAttached->colliders.erase(this); mainWorld.GameColliders.erase(this);*//* Clear();*/ }

	virtual void BeginPlay()override;

	virtual void EndPlay()override;

	virtual void Update(float deltaTime)override;

	/**
	 * ����������ȡ���и������ײ������ײ����������.
	 * 
	 * \param [in] type ������ 
	 * \return ��������
	 */
	const std::vector<Object*>& GetCollisionsByType(CollisionType type)
	{
		aims.clear();
		if (!collisions.empty())
		{
			for (auto it = collisions.begin(); it != collisions.end(); it++)
				if ((*it)->GetType() == type) { aims.push_back((*it)->GetOwner()); }
		}
		return aims;
	}

	/**
	 * ��ȡ��ײ�㼶.
	 * 
	 * \return �㼶 
	 */
	const int GetLayer()const { return layer; }
	/**
	 * ������ײ�㼶.
	 * 
	 * \param [in] layer ���õĲ㼶
	 */
	void SetLayer(int layer) { this->layer = layer; }

	/**
	 * ��ȡ��ײ��������.
	 */
	CollisionType GetType()const { return type; };
	/**
	 * ������ײ��������.
	 * @param [in] ���õ���ײ����
	 */
	void SetType(CollisionType type) { this->type = type; };

	/**
	 * ��ȡ��ײ��״.
	 */
	ColliderShape GetShape()const { return shape; };
	/**
	 * ������ײģʽ.
	 * 
	 * \param [in] mode ��ײģʽ
	 */
	void SetCollisionMode(ColliderMode mode) {
		if (mode == ColliderMode::None && this->mode != ColliderMode::None)
		{
			mainWorld.GameColliders_to_clear.insert(this); 
		}this->mode = mode; }
	/**
	 * ��ȡ��ײģʽ.
	 * 
	 * \return ��ײģʽ
	 */
	ColliderMode GetCollisionMode() { return mode; }

	/**
	 * ͣ����ײ��.
	 * @brief ������������ɾ��
	 */
	virtual void Deactivate()override;

	/**
	 * ���Ƶ�����
	 * _debug_ģʽִ��.
	 * 
	 */
	virtual void DrawDebugLine() = 0;

	///�Ƿ����κ���ײ���޽Ӵ�
	bool IsCollisionsEmpty()const { return collisions.size() == 0; }

	///�����������
	void SetPhysicsMaterial(const PhysicsMaterial& material) { this->material = material;  }

	//��ȡ���ο�
	Rect GetRect()const { return rec_pos; }

	//�¼�ί��
	CollisionOverlapDelegate OnComponentBeginOverlap;			///<��ײ��սӴ���ί��
	CollisionOverlapDelegate OnComponentEndOverlap;				///<��ײ��մӽӴ������ص���ί��
	CollisionHitOverlapDelegate OnComponentHitOverlap;			///<����ʵ����ײ��ί��

	//��ײ�����ȡ��Ҫ����
	static HitResult(*CollisionHitMap[3])(Collider*, Collider*);			///<ӳ���ϵ Բ��0 ����1 ���мӷ�
	static HitResult CollisionHitCircleToCircle(Collider*, Collider*);		///<Բʵ������֮�䷢��������ײ
	static HitResult CollisionHitCircleToBox(Collider*, Collider*);			///<Բʵ��ͺ���ʵ��֮�䷢��������ײ
	static HitResult CollisionHitBoxToBox(Collider*, Collider*);			///<���ͺͺ���ʵ��֮�䷢��������ײ

	HitResult CollisionHit(Collider* another);
protected:
	PhysicsMaterial material;			///<�������

	ColliderShape shape = ColliderShape::Circle;		///<��ײ��״
	
	/**
	 * ��ײ�ж�(���麯��).
	 * ������д
	 * \param [in] another ������ײ��
	 * \return �Ƿ�������ײ(���ص�)
	 */
	virtual bool CollisionJudge(Collider* another) = 0;

	/**
	 * �ж��Ƿ����귢������ײ.
	 * 
	 * \return ����Ƿ�����ײ���ڲ�
	 */
	virtual bool IsMouseOver() = 0;

	Rect rec_pos = {0,0,0,0};						///<���ο�����
private:
	Pair point = {-1,-1};			///<��ײ���Ͻ���������
	Pair point_rb = { -1,-1 };		///<��ײ���½���������

	///��ײ��������
	/// @brief ֻ�����ӳ���ϵ����ײ��������ײ
	CollisionType type = CollisionType::Default;
	///��ײͼ��˳��Խ���ֵԽ����
	int layer = 0;
	///��ײģʽ
	ColliderMode mode = ColliderMode::Trigger;

	///�洢������ײ������ ���������ص�����ײ�����ַ
	std::unordered_set<Collider*> collisions;
	///�洢������������ȡ�������ײ������ײ����������
	std::vector<Object*> aims;
	///������ײ������
	std::vector<Collider*> collisions_to_erase;

	RigidBody* rigidAttached = nullptr;			///<���ŵĸ������ ʵ������ģ��

	/**
	* ����ײ������糹���Ƴ�
	* @brief ��������ײ�� �����и����йص���ײ��.
	*
	*/
	void Clear();

	/**
	* ������ײ��֮�����Ϣɾ���߼�
	* @brief �Լ��ж��Լ��������ײ���Ƿ�����ײ.
	*
	*/
	void Erase();

	/**
	* ������ײ��֮�����Ϣ�����߼�.
	*
	* \param [in] another ��Ҫ�������ײ��
	*/
	void Insert(Collider* another);
};



/**
 * Բ����ײ��.
 */
class CircleCollider final :public Collider
{
	///�뾶
	float radius = 0;

	///��׼�뾶�ߴ��ģ
	float radius_ini = 0;
	
	/**
	 * ��ײ�ж�.
	 * @brief 1.��һ�����麯����д��Ϊ�˷������������ײ����״ 2.ȫ��д�ڻ������ӳ����Լ򻯺����ڲ�ʵ�֣������ж�֮ǰ�Ѿ�������
	 * \param [in] another ������ײ����ײ��
	 * \return  �Ƿ�������ײ
	 */
	virtual bool CollisionJudge(Collider* another)override;

public:
	/**
	 * ���캯��.
	 * @brief ��������ΪԲ��
	 */
	CircleCollider() { shape = ColliderShape::Circle; }

	virtual void BeginPlay() { Collider::BeginPlay(); };
	
	virtual bool IsMouseOver()override;
	virtual void Update(float deltaTime)override;
	/**
	 * ������ײ��ķ�Χ.
	 */
	virtual void DrawDebugLine();
	/**
	 * ��ȡ��ײ��뾶.
	 * 
	 * \return ���ذ뾶
	 */
	float GetRadius()const { return radius; }

	/**
	 * ���ð뾶.
	 * 
	 * \param [in] radius ���õİ뾶��С ����ֵ
	 */
	void SetRadius(float radius) 
	{ /*this->radius = std::abs(radius); this->radius_ini = radius / sqrtf(std::abs(GetWorldScale().x * GetWorldScale().y)); */
	this->radius = radius; this->radius_ini = radius / sqrtf(GetWorldScale().x * GetWorldScale().y);
	}
};

/**
 * ������ײ��.
 */
class BoxCollider final :public Collider
{
	
	Vector2D size = Vector2D(0, 0);			///<���ʹ�С
	
	Vector2D size_ini = Vector2D(0, 0);		//<������ײ���ķ��
	
	
	

	virtual bool CollisionJudge(Collider* another)override;
public:
	BoxCollider() { shape = ColliderShape::Box; }

	virtual void BeginPlay() { Collider::BeginPlay();/*Vector2D pos = GetWorldPosition();
	rec_pos = { pos.x - size.x / 2,pos.y - size.y / 2,pos.x + size.x / 2,pos.y + size.y / 2 };*/
	};

	virtual bool IsMouseOver()override;
	virtual void Update(float deltaTime)override;
	virtual void DrawDebugLine();
	
	/**
	 * ��ȡ��С.
	 * 
	 * \return ��С
	 */
	Vector2D GetSize()const { return size; }
	/**
	 * ���ô�С.
	 *
	 * \param [in] radius ���õĴ�С
	 */
	void SetSize(Vector2D size) { this->size = size; this->size_ini = size / GetWorldScale(); }

	
	/**
	 * ��ȡ�ص��ľ����С.
	 * @brief �贫���Ѿ�ȷ��������ײ��������ײ��
	 * \param [in] rect1  ���ο�1
	 * \param [in] rect2  ���ο�2
	 * \return �õ��ص�����
	 */
	static Vector2D GetOverlapRect(Rect rect1, Rect rect2);

	
};