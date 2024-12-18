/*****************************************************************//**
 * \file   Collider.h
 * \brief  游戏场景中所有碰撞相关事件的处理
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
 * 碰撞形状种类.
 * @remark 如果以后有新扩展的类，要在后面加上
 */
enum class ColliderShape :uint8{
Circle			///<圆型碰撞
, Box			///<盒型碰撞
};

/**
 * 碰撞模式.
 * @brief 碰撞器的模式，会产生不同的效果
 */
enum class ColliderMode:uint8{
	None		///<没有模式，相当于没有这个碰撞器，不会产生碰撞判断
	,Trigger	///<触发模式,只会发生碰撞然后发生事件
	,Collision	///<碰撞模式，相当于两个实体的球进行碰撞
};


/**
 * 碰撞结果类
 * @remark 返回物理需要的信息.
 */
struct  HitResult
{
	Vector2D impactpoint;			///<切点
	Vector2D impactnormalunit;		///<力法线向量单位向量 对碰撞到的物体
	Object* hitobject;				///<击中的物体
	Component* hitcomponent;		///<击中的组件

	HitResult() :impactpoint(0, 0), impactnormalunit(0, 0), hitobject(nullptr), hitcomponent(nullptr) {};
	HitResult(const Vector2D& point, const Vector2D& tangnormal, Object* object, Component* component) :
		impactpoint(point), impactnormalunit(tangnormal), hitobject(object), hitcomponent(component) {};
};

class Collider;

/**
 * 碰撞重叠事件委托.
 * @remark 类参数[Collider* overlapcomponentself,Collider* othercomponent,Object* otherobject]
 */
DECLARE_MULTI_PARAM_MULTI_DELEGATE_CLASS(CollisionOverlapDelegate, Collider*, Collider*, Object*)

/**
 * 物理模拟物体碰撞事件委托.
 * 类参数[Collider* overlapcomponentself,Collider* othercomponent,Object* otherobject,Vector2D normalpurse 法向方向的力,const HitResult& hitresult]
 */
DECLARE_MULTI_PARAM_MULTI_DELEGATE_CLASS(CollisionHitOverlapDelegate, Collider*, Collider*, Object*,Vector2D,const HitResult&)



/**
 * 碰撞委托(第一版本）.
 * @brief 特写 就是碰撞的时候执行相关委托函数，得到参数给函数
 * 得到函数执行控制权
 * @see MultiCastDelegate
 */
class CollisionDelegate
{
	std::vector <std::function<void(Collider*,Object*)>>callbacks;		///封装多个回调函数 获得跟他碰撞的碰撞器和物体

public:
	///添加函数
	template<typename T>
	void Add(T* obj, void(T::* func)(Collider*, Object*))
	{
		callbacks.push_back(std::bind(func, obj, std::placeholders::_1, std::placeholders::_2));
	}
	///添加函数
	void Add(std::function<void(Collider*, Object*)>func)
	{
		callbacks.push_back(func);
	}

	///移除相应委托
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

	///移除相应委托
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

	///发布委托(执行所有委托)
	void BroadCast(Collider* OverlapCollider,Object* OverlapActor);

};


/**
 * 碰撞器基类.
 * @brief 实现碰撞.实现一些碰撞共有基本逻辑的处理和声明周期的管理
 * @remark 新的碰撞器类型都要继承他
 */
class Collider :public SceneComponent
{
	friend class Controller;
	friend class RigidBody;
	friend void World::ProcessColliderZones();
	/*friend class World;*/
	
public:

	/**
	 * 构造函数.
	 * 世界碰撞容器中插入
	 */
	Collider() :type(CollisionType::Default),mode(ColliderMode::Trigger){ mainWorld.GameColliders.insert(this); }
	virtual ~Collider() { /*if (rigidAttached)rigidAttached->colliders.erase(this); mainWorld.GameColliders.erase(this);*//* Clear();*/ }

	virtual void BeginPlay()override;

	virtual void EndPlay()override;

	virtual void Update(float deltaTime)override;

	/**
	 * 由种类名获取所有跟这个碰撞体有碰撞的物体容器.
	 * 
	 * \param [in] type 种类名 
	 * \return 数组容器
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
	 * 获取碰撞层级.
	 * 
	 * \return 层级 
	 */
	const int GetLayer()const { return layer; }
	/**
	 * 设置碰撞层级.
	 * 
	 * \param [in] layer 设置的层级
	 */
	void SetLayer(int layer) { this->layer = layer; }

	/**
	 * 获取碰撞物体种类.
	 */
	CollisionType GetType()const { return type; };
	/**
	 * 设置碰撞物体种类.
	 * @param [in] 设置的碰撞种类
	 */
	void SetType(CollisionType type) { this->type = type; };

	/**
	 * 获取碰撞形状.
	 */
	ColliderShape GetShape()const { return shape; };
	/**
	 * 设置碰撞模式.
	 * 
	 * \param [in] mode 碰撞模式
	 */
	void SetCollisionMode(ColliderMode mode) {
		if (mode == ColliderMode::None && this->mode != ColliderMode::None)
		{
			mainWorld.GameColliders_to_clear.insert(this); 
		}this->mode = mode; }
	/**
	 * 获取碰撞模式.
	 * 
	 * \return 碰撞模式
	 */
	ColliderMode GetCollisionMode() { return mode; }

	/**
	 * 停用碰撞体.
	 * @brief 从世界容器中删除
	 */
	virtual void Deactivate()override;

	/**
	 * 绘制调试线
	 * _debug_模式执行.
	 * 
	 */
	virtual void DrawDebugLine() = 0;

	///是否与任何碰撞体无接触
	bool IsCollisionsEmpty()const { return collisions.size() == 0; }

	///设置物理材质
	void SetPhysicsMaterial(const PhysicsMaterial& material) { this->material = material;  }

	//获取矩形框
	Rect GetRect()const { return rec_pos; }

	//事件委托
	CollisionOverlapDelegate OnComponentBeginOverlap;			///<碰撞体刚接触的委托
	CollisionOverlapDelegate OnComponentEndOverlap;				///<碰撞体刚从接触到不重叠的委托
	CollisionHitOverlapDelegate OnComponentHitOverlap;			///<发生实体碰撞的委托

	//碰撞击打获取必要参数
	static HitResult(*CollisionHitMap[3])(Collider*, Collider*);			///<映射关系 圆是0 方是1 进行加法
	static HitResult CollisionHitCircleToCircle(Collider*, Collider*);		///<圆实体类型之间发生物理碰撞
	static HitResult CollisionHitCircleToBox(Collider*, Collider*);			///<圆实体和盒型实体之间发生物理碰撞
	static HitResult CollisionHitBoxToBox(Collider*, Collider*);			///<盒型和盒型实体之间发生物理碰撞

	HitResult CollisionHit(Collider* another);
protected:
	PhysicsMaterial material;			///<物理材质

	ColliderShape shape = ColliderShape::Circle;		///<碰撞形状
	
	/**
	 * 碰撞判断(纯虚函数).
	 * 子类重写
	 * \param [in] another 检测的碰撞体
	 * \return 是否发生了碰撞(即重叠)
	 */
	virtual bool CollisionJudge(Collider* another) = 0;

	/**
	 * 判断是否跟鼠标发生了碰撞.
	 * 
	 * \return 鼠标是否在碰撞体内部
	 */
	virtual bool IsMouseOver() = 0;

	Rect rec_pos = {0,0,0,0};						///<矩形框区域
private:
	Pair point = {-1,-1};			///<碰撞左上角区域坐标
	Pair point_rb = { -1,-1 };		///<碰撞右下角区域坐标

	///碰撞物体种类
	/// @brief 只会跟有映射关系的碰撞器发生碰撞
	CollisionType type = CollisionType::Default;
	///碰撞图层顺序，越大的值越上面
	int layer = 0;
	///碰撞模式
	ColliderMode mode = ColliderMode::Trigger;

	///存储发生碰撞的容器 就是区域重叠的碰撞组件地址
	std::unordered_set<Collider*> collisions;
	///存储所有种类名获取跟这个碰撞体有碰撞的物体容器
	std::vector<Object*> aims;
	///结束碰撞缓冲区
	std::vector<Collider*> collisions_to_erase;

	RigidBody* rigidAttached = nullptr;			///<附着的刚体组件 实现物理模拟

	/**
	* 将碰撞体从世界彻底移除
	* @brief 清空这个碰撞体 及所有跟他有关的碰撞体.
	*
	*/
	void Clear();

	/**
	* 处理碰撞体之间的信息删除逻辑
	* @brief 自己判断自己里面的碰撞体是否还有碰撞.
	*
	*/
	void Erase();

	/**
	* 处理碰撞体之间的信息插入逻辑.
	*
	* \param [in] another 需要插入的碰撞器
	*/
	void Insert(Collider* another);
};



/**
 * 圆形碰撞器.
 */
class CircleCollider final :public Collider
{
	///半径
	float radius = 0;

	///标准半径尺寸规模
	float radius_ini = 0;
	
	/**
	 * 碰撞判断.
	 * @brief 1.第一种用虚函数重写是为了分类跟它发生碰撞的形状 2.全部写在基类进行映射可以简化函数内部实现，发生判断之前已经分类了
	 * \param [in] another 发生碰撞的碰撞体
	 * \return  是否发生了碰撞
	 */
	virtual bool CollisionJudge(Collider* another)override;

public:
	/**
	 * 构造函数.
	 * @brief 种类设置为圆形
	 */
	CircleCollider() { shape = ColliderShape::Circle; }

	virtual void BeginPlay() { Collider::BeginPlay(); };
	
	virtual bool IsMouseOver()override;
	virtual void Update(float deltaTime)override;
	/**
	 * 绘制碰撞体的范围.
	 */
	virtual void DrawDebugLine();
	/**
	 * 获取碰撞体半径.
	 * 
	 * \return 返回半径
	 */
	float GetRadius()const { return radius; }

	/**
	 * 设置半径.
	 * 
	 * \param [in] radius 设置的半径大小 是正值
	 */
	void SetRadius(float radius) 
	{ /*this->radius = std::abs(radius); this->radius_ini = radius / sqrtf(std::abs(GetWorldScale().x * GetWorldScale().y)); */
	this->radius = radius; this->radius_ini = radius / sqrtf(GetWorldScale().x * GetWorldScale().y);
	}
};

/**
 * 盒型碰撞器.
 */
class BoxCollider final :public Collider
{
	
	Vector2D size = Vector2D(0, 0);			///<盒型大小
	
	Vector2D size_ini = Vector2D(0, 0);		//<盒型碰撞器的风格
	
	
	

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
	 * 获取大小.
	 * 
	 * \return 大小
	 */
	Vector2D GetSize()const { return size; }
	/**
	 * 设置大小.
	 *
	 * \param [in] radius 设置的大小
	 */
	void SetSize(Vector2D size) { this->size = size; this->size_ini = size / GetWorldScale(); }

	
	/**
	 * 获取重叠的矩阵大小.
	 * @brief 需传入已经确定发生碰撞的两个碰撞器
	 * \param [in] rect1  矩形框1
	 * \param [in] rect2  矩形框2
	 * \return 得到重叠矩形
	 */
	static Vector2D GetOverlapRect(Rect rect1, Rect rect2);

	
};