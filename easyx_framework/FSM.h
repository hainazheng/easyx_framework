/*****************************************************************//**
 * \file   FSM.h
 * \brief  分层状态机
 * @remark 使用树结构，可以用来实现更加模块化复杂化的AI,行为树等
 * 
 * \author zhn
 *********************************************************************/

#pragma once
#include"CoreMinimal.h"
#include"Delegate.h"
#include<unordered_map>
#include<map>
#include<chrono>
#include"Collider.h"
#include<stack>
#include<list>
#include"CondiEdge.h"
#include"Log.h"

////角色状态
//enum class role_status:uint8
//{
//	attack,
//	idle,
//	dead,
//	escape
//};
//
////行为状态
//enum class action_status :uint8
//{
//	walk,
//	rush,
//	jump,
//	attack,
//	stop
//
//};

////枚举所有可能的状态
//enum class role_Events :uint8
//{
//	event1,
//	event2,
//	event3
//};
//enum class action_Events :uint8
//{
//	event1,
//	event2,
//	event3,
//	event4,
//	event5
//};

///动作委托类
DECLARE_NO_PARAM_UNICAST_DELEGATE(action)

////状态项
//class FSMItems
//{
//	friend class FSM;
//	role_status current_rolestatus;		//当前角色层状态
//	action_status current_actionstatus;	//当前行为层状态
//	role_status next_rolestatus;			//角色层次态
//	action_status next_actionstatus;			//行为层次态
//	action action_role;						//角色动作
//	action action_act;						//行为动作
//	role_Events role_event;							//角色事件
//	action_Events act_event;						//行为事件
//public:
//	//构造函数
//	FSMItems(role_status _current_rolestatus,
//	action_status _current_actionstatus,
//	role_status _next_rolestatus,
//	action_status _next_actionstatus,
//	action _action_role,
//	action _action_act,
//	role_Events _role_event,
//	action_Events _act_event):
//		current_rolestatus(_current_rolestatus),
//		current_actionstatus(_current_actionstatus),
//		next_rolestatus(_next_rolestatus),
//		next_actionstatus(_next_actionstatus),
//		action_role(_action_role),
//		action_act(_action_act),
//		role_event(_role_event),
//		act_event(_act_event){}
//
//
//
//};


////状态基类
//class status
//{
//	virtual void update() = 0;
//	virtual void enter() = 0;
//	virtual void exit() = 0;
//};

///状态值排序规则，按照数值正序排序
struct StatusSort
{
	bool operator()(const class FSMItems* a, const class FSMItems* b)const;
};

//基本逻辑
/*        一颗树存储所有状态          */
/*         同一层级的节点可以转换的     */
/*         节点有自己的事件    */

/**
 * 状态基类.
 * @remark 基本逻辑:
 * @remark 一颗树存储所有状态， 同一层级的节点可以转换的， 节点有自己的事件
 * @remark !!!想要使用就要重写自己的状态
 */
class FSMItems
{
	DECLARE_EDGE_CONDITION_BY_TYPE(FSMEdge,FSMItems)

	friend class FSM;
	friend class FSMEdge;
protected:
	int level;					///<根深度
	/*std::unordered_set<std::string> a;*/
	int status;			///<状态值

	std::set< FSMItems*,StatusSort> children;			///<孩子状态容器

	FSMItems* parent=nullptr;			///<父节点状态 

	std::list<FSMEdge*> nexts;			///<状态条件桥
public:
	/**
	 * 构造函数.
	 * @remark 一般都不用用户管理 可选父节点
	 * @see FSM::AddStatus
	 * \param [in] _status  状态值
	 * \param [in] _parent  父节点
	 * \param [in] _level	节点深度，
	 */
	FSMItems(int _status, FSMItems* _parent = nullptr, int _level = 0) :
		status(_status)
	{

		AttachTo(_parent);
		level = _level;
		//自动推导节点深度
		if (parent != nullptr)
		{
			level = parent->level+1;
		}
		
	}
	/**
	 * 析构函数.
	 * 节点删除基本由状态机管理
	 */
	~FSMItems()
	{
		if (parent)parent->children.erase(this);
	}

	/**
	 * 设置父节点.
	 * @brief 绑定 并设置新状态值
	 * \param  [in] par 父节点
	 */
	void AttachTo(FSMItems* par)
	{
		DetachForm(this->parent);
		if (par)
		{
			parent = par;
			level = parent->level+1;
			status = parent->newchildstatus();
			par->children.insert(this);
		
		}
	}
		
	/**
	 * 解除绑定.
	 * @remark 不建议用户使用
	 * \param [in] par 父节点
	 */
	void DetachForm(FSMItems* par)
	{
		if (par)
		{
			par->children.erase(this);
			if (par == parent)parent = nullptr;
			level = 0;
		}
	}

	/**
	 * 删除节点状态.
	 * 
	 */
	void Destory();

	/**
	 * 获取还没有的子节点状态值.
	 * 
	 * \return 下一个子节点状态值
	 */
	int newchildstatus()
	{
		//寻找子孩子中是否有状态值
		int new_status = 0;
		for (auto it : children)
		{
			if (it->GetStatus() == new_status ) { new_status++; }
			else { break; }
		}
		return new_status;
	}

	/**
	 * 获取状态值.
	 * 
	 * \return 状态值
	 */
	int GetStatus()const
	{
		return status;
	}

	FSM* fsm = nullptr;				///<绑定的状态机
	Animator* ani = nullptr;		///<同物体的动画组件
	Collider* col = nullptr;		///<同物体的碰撞组件

	action act;				///<执行事件(每帧执行) 一般用户重写新节点就是添加事件
	action enter;			///<进入事件 一般用户重写新节点就是添加事件
	action exit;			///<退出事件 一般用户重写新节点就是添加事件
};

/**
 * FSM需要的参数.
 * 
 * 想要哪些可以加进去
 */
struct FSM_Info
{
	Animator* ani = nullptr;		///<动画播放器 默认是绑定的物体的播放器 由他进行渲染

	//time_point<steady_clock> clock;		//计时
	Collider* col = nullptr;		///<绑定的碰撞体
	
};

/**
 * 状态机组件.
 * @brief 执行状态的管理
 */
class FSM final:public Component
{
	DECLARE_EDGE_CONDITION_BY_TYPE(FSMEdge, FSMItems)
public:
	
	/**
	 * 添加节点状态 
	 * @brief默认状态并行.
	 * @remark !!!用户添加创建新节点要用这个 
	 * \param [in] name 状态节点名字
	 * \param [in] parent 父节点
	 * \return 新节点的指针
	 */
	template<class T>
	T* AddStatus(std::string name, std::string parent = "root")
	{
		////如果已经存在了当前节点
		//if (status.find(name) != nullptr)return nullptr;

		//寻找状态机是否有父节点，一定要绑定在一个父节点的下面
		FSMItems* it = FindStatusByName(parent);
		if (!it) {
			Log::instance()->printf(Log::LEVEL_ERR, __FILE__, __LINE__,
				"父节点:%s不存在", parent.c_str()); return nullptr;
		}	


		//创建节点
		T* item = new T(it->newchildstatus(), it);
	
		if (item && static_cast<FSMItems*>(item))		//用静态转化是因为子类转化为父类，不能父类转化成子类
		{
			item->fsm = this;
			item->ani = this->fsm_info.ani;
			item->col = this->fsm_info.col;

			//映射
			status.insert({ name,item });
			return item;
		}
		Log::instance()->printf(Log::LEVEL_ERR, __FILE__, __LINE__,
			"创建节点状态:%s失败", name.c_str());
		delete item;
		return nullptr;
	};

	/**
	 * 移除节点状态
	 * @brief 默认删除所有子节点状态.
	 * 
	 * \param [in] name 状态名
	 * \param [in] delchild  是否删除所有子节点
	 */
	void RemoveStatus(std::string name,bool delchild=1);
private:
	/**
	 * 注册状态在状态机.
	 * 
	 * \param [in] name 状态名
	 * \param [in] item 状态节点
	 */
	void RegisterStatus(std::string name,FSMItems* item)
	{
		status.insert({name,item});
	}

	/**
	 * 递归从上到下执行行为.
	 * 
	 * \param [in] item 状态节点 
	 */
	void process_Update(FSMItems* item);

	/**
	 * 递归从上到下执行进入事件.
	 * 
	 * \param  [in] item 执行节点
	 * \param  [in]  end 从哪个上层节点开始
	 */
	void process_Enter(FSMItems* item, FSMItems* end = nullptr);

	/**
	 * 递归从下到上执行退出事件.
	 * 
	 * \param [in] item 执行节点
	 * \param [in]  end 执行到哪个上层节点为止
	 */
	void process_Exit(FSMItems* item, FSMItems* end = nullptr);

	/**
	 * 每一帧执行行为.
	 * @remark 1.若无过渡条件且有边 或者2.已经符合条件，则执行一遍行为结束立马到下一个节点
	 * \param [in] deltaTime 帧间隔时间
	 */
	virtual void Update(float deltaTime);

	/**
	 * 清空某个节点子状态(也包括当前节点).
	 * 
	 * \param [in] name 状态节点名
	 */
	void Clear(std::string name);

	/**
	 * 取消注册状态在状态机.
	 * 
	 * \param [in] name 状态节点名
	 */
	void UnregisterStatus(std::string name)
	{
		auto it = status.find(name);		
		status.erase(it);			
	}
	/**
	 * 取消注册状态在状态机.
	 *
	 * \param [in] name 状态节点
	 */
	void UnregisterStatus(FSMItems* item)
	{
		for (auto &it : status)
		{
			if (it.second == item)
			{
				status.erase(it.first);
				return;
			}
		}
		
	}
public:
	/**
	 * 得到当前执行的状态节点.
	 * 
	 * \return 状态节点
	 */
	FSMItems* GetNormalStatus()
	{
		return normalstatus;
	}

	/**
	 * 得到上一次执行的状态名.
	 * 
	 * \return 状态名
	 */
	std::string GetLastStatus()
	{
		return laststatus;
	}

	/**
	 * 获取状态节点名字.
	 * 
	 * \param [in] item 状态节点
	 * \return 状态名字
	 */
	std::string getNameStatus(FSMItems* item)
	{
		if (!item)return std::string("null");
		for(auto& it:status)
		{
			if ((it.second) == (item))return it.first;
		
		}
		return std::string("null");
	}

	/**
	 * 由名字查找状态节点.
	 * 
	 * \param [in] name 状态名字 
	 * \return 状态节点
	 */
	FSMItems* FindStatusByName(std::string name)
	{
		auto it=status.find(name);
		/*auto it = status.begin();*/
		if (it != status.end())
		{
			return it->second;
		}
		return nullptr;
	}

	/**
	 * 加载一些参数.
	 * @remark FSM_Info类添加了哪些也要加上
	 * @remark 要动态加载就写在update里面
	 */
	virtual void BeginPlay()
	{
		//加载信息
	fsm_info.ani = GetOwner()->GetComponentByClass<Animator>(); 
	fsm_info.col= GetOwner()->GetComponentByClass<Collider>();
	}

	////由状态查找名字
	//std::string GetName()const { for (auto& it : status) { if (&(it.second) == (aninode))return it.first; }; }
private:
	/**
	 * 寻找最近公共祖先.
	 * 
	 * \param [in] root 根状态节点 其中的一个公共祖先
	 * \param [in] p   第一个状态节点
	 * \param [in] q   第二个状态节点
	 * \return  最近公共祖先节点
	 */
	FSMItems* lowestCommonAncestor(FSMItems* root, FSMItems* p, FSMItems* q);

public:
	/**
	 * 获取节点路径.
	 * @brief 用链表满足先进先出
	 * \param [in] item 状态节点
	 * \return  路径节点链表 存储从自己开始到根节点 
	 */
	std::list<FSMItems*> GetPath(FSMItems* item)
	{
	std::list<FSMItems*> path;
	if (!item)return path;
		
		FSMItems* it = item;
		while (it != nullptr)
		{
			path.push_back(it);
			it = it->parent;
		}
		return path;
	}

	/**
	 * 切换状态.
	 * @remark 切换状态调用的函数
	 * @remark ！！！除了第一次进入状态，不建议在程序中直接使用
	 * @remark 节点切换先初始化边条件 再添加条件同时在状态机添加同样名字的实参
	 * @see CondiEdge
	 * \param [in] name 状态名
	 */
	void TransitionState(std::string name)
	{

		if (normalstatus != nullptr)
		{
			process_Exit(normalstatus,lowestCommonAncestor(root, status[name],normalstatus));
		
		}
		laststatus = getNameStatus(normalstatus);
		normalstatus = status[name];
		
		if (normalstatus)
		{
			process_Enter(normalstatus, lowestCommonAncestor(root, status[name], normalstatus));

		}
	}

	/**
	 * 切换状态.
	 * @remark 切换状态调用的函数
	 * \param [in] item 状态节点
	 */
	void TransitionState(FSMItems* item)
	{

		if (normalstatus != nullptr)
		{
			process_Exit(normalstatus, lowestCommonAncestor(root, item, normalstatus));

		}
		laststatus = getNameStatus(normalstatus);
		normalstatus = item;

		if (normalstatus)
		{
			process_Enter(normalstatus, lowestCommonAncestor(root, item, normalstatus));

		}
	}

	/**
	 * 初始化.
	 * @brief 主要是为了以后作为一个独立的类需要的函数 实现更加复杂的管理
	 */
	void Init() {};

	/**
	 * 销毁状态机.
	 * 
	 */
	void Destruct();

	/**
	 * 唤醒状态树.
	 * 组件用不到
	 */
	void Awake()
	{
		Init();
	}

	/*FSM(std::string name) 
	{ fsm_info.ani = GetOwner()->GetComponentByClass<Animator>();
	root = new FSMItems(0); status.insert({ "root",root }); root->fsm = this;
	root->ani = this->fsm_info.ani; AddStatus<>(name, "root"); Awake();
	TransitionState(name); }*/

	/**
	 * 构造函数.
	 * @brief主要就是创建根节点
	 */
	FSM() 
	{
		
		root = new FSMItems(0); status.insert({ "root",root }); root->fsm = this; 
		  TransitionState("root");
	}

	/**
	 * 析构函数
	 * 销毁所有节点回收资源.
	 * 
	 */
	~FSM() { Destruct(); }

	/**
	 * 添加参数到实际值容器中.
	 * @remark 只有容器中有这个参数名才能设置或修改值
	 * \param [in] paramName 参数名字
	* \param [in] type  状态参数类型
	*/
	void AddParamater(std::string paramName, ParamType type);

	/**
	 * 设置或修改参数.
	 *@remark 要先AddParamater再调用这个
	 * \param [in] paramName 参数名字
	 * \param [in] value T类型数值
	 */
	template<class T>
	void SetValue(std::string paramName, T&& _value) 
	{
		using Type = std::decay_t<decltype(_value)>;
		if (std::is_same<Type, int>::value)
		{
			if (parametersInteger.find(paramName) != parametersInteger.end())
		 {
				parametersInteger[paramName] = std::forward<T>(_value); return;
		 }
		}
		if (std::is_same<Type, float>::value|| std::is_same<Type, double>::value)
		{
			if (parametersFloat.find(paramName) != parametersFloat.end())
			{
				parametersFloat[paramName] = std::forward<T>(_value); return;
			}
		}
		if (std::is_same<Type,bool >::value)
		{
			if (parametersBool.find(paramName) != parametersBool.end())
			{
				parametersBool[paramName] = std::forward<T>(_value); return;
			}
		}
		// 处理其他类型的情况
		Log::instance()->printf(Log::LEVEL_WAR, __FILE__, __LINE__,
			"Unsupported type for parameter: %s", paramName);
	};

	/**
	 * 启动触发器.
	 *
	 * \param [in] paramName 参数名字
	 */
	void SetTrigger(std::string paramName);

	/**
	 * 是否符合过渡条件.
	 *
	 * \param edge 动画过渡类
	 * \return    true满足条件 false不满足条件
	 */
	bool CheckConditions(FSMEdge* edge);

private:
	/**
	 * 节点映射 [名字，状态节点]
	 * 用名称访问节点.
	 */
	std::unordered_map<std::string, FSMItems*> status;
	///根节点
	FSMItems* root ;
	///现在状态节点
	FSMItems* normalstatus=nullptr;

	/**
	 * 前一状态 
	 * @remark 没有则为null 或者前一个删除了.
	 */
	std::string laststatus = "null";

	/**
	 * 需要的参数.
	 */
	FSM_Info fsm_info;

	//存放所有实参
	std::unordered_map<std::string, int> parametersInteger;				///<实际整形数值条件容器 [条件参数名字，实际数值]
	std::unordered_map<std::string, float> parametersFloat;				///<实际浮点型数值条件容器 [条件参数名字，实际数值]		
	std::unordered_map<std::string, bool> parametersBool;				///<实际布尔型数值条件容器 [条件参数名字，实际数值]
	std::unordered_map<std::string, bool> parametersTrigger;			///<实际触发型数值条件容器 [条件参数名字，实际数值]
};