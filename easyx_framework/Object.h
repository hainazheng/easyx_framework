/*****************************************************************//**
 * \file   Object.h
 * \brief  游戏对象基类
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include<unordered_set>
#include"CoreMinimal.h"
#include"Log.h"
#include"SceneComponent.h"

class Component;
class SceneComponent;

/**
 * 游戏对象基类.
 */
class Object :public base
{
	std::unordered_set<Object*> children;									///<孩子容器
	Object* parent = nullptr;									///<实现物体之间的绑定  例如 有一个火焰粒子绑定在物体上实现一起移动实现特效

	std::unordered_set<Component*> components;						///<容器存储指针其实是地址，指针无所谓存不存在，只要取出的地址给一个新的指针就行
	std::unordered_set<Component*>::iterator components_iter;			///<控制组件的遍历和增删

protected:
	SceneComponent* const root = new SceneComponent;							///<场景根组件，赋予场景属性  刚创建物体一定同时创建了它的场景组件,不允许改变指向

public:
	///只是一个场景根组件(带有场景属性)，不需要遍历，所以不需要加入容器
	Object() { root->SetOwner(this); }				
	///组件删除的控制权在对象中
	~Object()																
	{
		for (auto& com : components)
		{

			/*std::cout << components.size()<<std::endl;*/
			delete com;
		}
		delete root;
	}

	///遍历运行每个组件
	virtual void Update(float deltaTime)override				
	{

		components_iter = components.begin();
		while (components_iter != components.end())
		{
			(*components_iter)->Update(deltaTime);
			if (components_iter == components.end())break;			//防止增删产生的迭代器的变动问题 只要指向超尾的最后一个就跳出循环
			components_iter++;

		}
	}

	///设置虚函数，把不能写在构造却需要优先实现的写在这
	virtual void BeginPlay();

	virtual void EndPlay();

	/**
	 * 在物体容器中注册组件.
	 * @remark ！！！用户慎用
	 * \param [in]pCom 组件指针（组件基类极其子类）
	 */
	void RegisterComponent(Component* pCom)
	{
		components.insert(pCom);
	}

	/**
	 * 从物体组件容器中删除相应组件.
	 * @remark ！！！用户慎用
	 * \param [in] pCom 组件指针（组件基类极其子类）
	 */
	void UnregisterComponent(Component* pCom)
	{
		auto it = components.find(pCom);		//从容器中寻找组件
		components_iter = components.erase(it);			//返回删除之后的后继节点
	}

	/**
	 * 访问物体对象的跟他匹配的组件.
	 * 模板函数<组件基类极其子类>
	 * \return 找到的返回相应组件指针 没有就nullptr
	 */
	template<class T>
	T* GetComponentByClass()
	{
		for (auto& com : components)
		{
			if (T* pCom = dynamic_cast<T*>(com))return pCom;			//只要容器中取出的对象能转化成需要的类型，就把他返回
		}
		return nullptr;
	}

	/**
	 * 创建组件，绑定到对象，并注册到对象容器.
	 * 模板函数<组件基类极其子类>
	 * 相当于工厂组装好的创建组件 
	 * ！！！统一物体用这个创建组件
	 * \return  成功返回相应组件指针 失败就nullptr
	 */
	template<class T>
	T* ConstructComponent()
	{
		T* pCom = new T;
		if (pCom && static_cast<Component*>(pCom))		//用静态转化是因为子类转化为父类，不能父类转化成子类
		{
			pCom->SetOwner(this);
			RegisterComponent(pCom);
			pCom->BeginPlay();
			return pCom;
		}
		delete pCom;
		Log::instance()->printf(Log::LEVEL_WAR, __FILE__, __LINE__,
			"创建组件失败");
		return nullptr;
	}

	/**
	 * 设置所属物体 实现物体绑定.
	 * 
	 * \param [in] par 父物体指针
	 */
	void AttachTo(Object* par);
	/**
	 * 解除所属物体 解除物体绑定.
	 * ！！！慎用
	 * \param [in] par 父指针
	 */
	void DetachForm(Object* par);

	/**
	 * 设置场景根组件，物体以哪个组件场景属性作为自己的场景属性.
	 * 
	 * \param [in] newroot 新的根组件
	 */
	void SetRootComponent(SceneComponent* newroot);

	void Destroy();			//用深度优先遍历实现遍历到世界类的删除容器中

	///获取物体相对坐标
	Vector2D GetLocalPosition() const {
		if (root != nullptr) {/* std::cout << 1 << std::endl;*/return root->GetLocalPosition(); }
		return Vector2D(0, 0);
	}		
	///获取物体相对角度
	float GetLocalRotation() const { if (root != nullptr)return root->GetLocalRotation(); return 0; }			
	///获取物体相对缩放大小
	Vector2D GetLocalScale() const { if (root != nullptr)return root->GetLocalScale(); return Vector2D(0, 0); }			
	Vector2D GetWorldPosition() const;		//获取物体绝对坐标
	float GetWorldRotation() const;			//获取物体绝对角度
	Vector2D GetWorldScale() const;			//获取物体绝对缩放大小

	///设置物体相对坐标
	void SetLocalPosition(const Vector2D& pos) { if (root != nullptr)root->SetLocalPosition(pos); }			
	///设置物体相对角度
	void SetLocalRotation(float rot) { if (root != nullptr)root->SetLocalRotation(rot); }					
	///设置物体相对缩放大小
	void SetLocalScale(const Vector2D& scale) { if (root != nullptr)root->SetLocalScale(scale); }			
	
	///加上一个坐标
	void AddPosition(const Vector2D& pos) { root->AddPosition(pos); }			
	///加上一个角度
	void AddRotation(float rot) { root->AddRotation(rot); }						

	virtual void DrawDebugPosition()const;
};