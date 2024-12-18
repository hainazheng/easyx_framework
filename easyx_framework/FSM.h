/*****************************************************************//**
 * \file   FSM.h
 * \brief  �ֲ�״̬��
 * @remark ʹ�����ṹ����������ʵ�ָ���ģ�黯���ӻ���AI,��Ϊ����
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

////��ɫ״̬
//enum class role_status:uint8
//{
//	attack,
//	idle,
//	dead,
//	escape
//};
//
////��Ϊ״̬
//enum class action_status :uint8
//{
//	walk,
//	rush,
//	jump,
//	attack,
//	stop
//
//};

////ö�����п��ܵ�״̬
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

///����ί����
DECLARE_NO_PARAM_UNICAST_DELEGATE(action)

////״̬��
//class FSMItems
//{
//	friend class FSM;
//	role_status current_rolestatus;		//��ǰ��ɫ��״̬
//	action_status current_actionstatus;	//��ǰ��Ϊ��״̬
//	role_status next_rolestatus;			//��ɫ���̬
//	action_status next_actionstatus;			//��Ϊ���̬
//	action action_role;						//��ɫ����
//	action action_act;						//��Ϊ����
//	role_Events role_event;							//��ɫ�¼�
//	action_Events act_event;						//��Ϊ�¼�
//public:
//	//���캯��
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


////״̬����
//class status
//{
//	virtual void update() = 0;
//	virtual void enter() = 0;
//	virtual void exit() = 0;
//};

///״ֵ̬������򣬰�����ֵ��������
struct StatusSort
{
	bool operator()(const class FSMItems* a, const class FSMItems* b)const;
};

//�����߼�
/*        һ�����洢����״̬          */
/*         ͬһ�㼶�Ľڵ����ת����     */
/*         �ڵ����Լ����¼�    */

/**
 * ״̬����.
 * @remark �����߼�:
 * @remark һ�����洢����״̬�� ͬһ�㼶�Ľڵ����ת���ģ� �ڵ����Լ����¼�
 * @remark !!!��Ҫʹ�þ�Ҫ��д�Լ���״̬
 */
class FSMItems
{
	DECLARE_EDGE_CONDITION_BY_TYPE(FSMEdge,FSMItems)

	friend class FSM;
	friend class FSMEdge;
protected:
	int level;					///<�����
	/*std::unordered_set<std::string> a;*/
	int status;			///<״ֵ̬

	std::set< FSMItems*,StatusSort> children;			///<����״̬����

	FSMItems* parent=nullptr;			///<���ڵ�״̬ 

	std::list<FSMEdge*> nexts;			///<״̬������
public:
	/**
	 * ���캯��.
	 * @remark һ�㶼�����û����� ��ѡ���ڵ�
	 * @see FSM::AddStatus
	 * \param [in] _status  ״ֵ̬
	 * \param [in] _parent  ���ڵ�
	 * \param [in] _level	�ڵ���ȣ�
	 */
	FSMItems(int _status, FSMItems* _parent = nullptr, int _level = 0) :
		status(_status)
	{

		AttachTo(_parent);
		level = _level;
		//�Զ��Ƶ��ڵ����
		if (parent != nullptr)
		{
			level = parent->level+1;
		}
		
	}
	/**
	 * ��������.
	 * �ڵ�ɾ��������״̬������
	 */
	~FSMItems()
	{
		if (parent)parent->children.erase(this);
	}

	/**
	 * ���ø��ڵ�.
	 * @brief �� ��������״ֵ̬
	 * \param  [in] par ���ڵ�
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
	 * �����.
	 * @remark �������û�ʹ��
	 * \param [in] par ���ڵ�
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
	 * ɾ���ڵ�״̬.
	 * 
	 */
	void Destory();

	/**
	 * ��ȡ��û�е��ӽڵ�״ֵ̬.
	 * 
	 * \return ��һ���ӽڵ�״ֵ̬
	 */
	int newchildstatus()
	{
		//Ѱ���Ӻ������Ƿ���״ֵ̬
		int new_status = 0;
		for (auto it : children)
		{
			if (it->GetStatus() == new_status ) { new_status++; }
			else { break; }
		}
		return new_status;
	}

	/**
	 * ��ȡ״ֵ̬.
	 * 
	 * \return ״ֵ̬
	 */
	int GetStatus()const
	{
		return status;
	}

	FSM* fsm = nullptr;				///<�󶨵�״̬��
	Animator* ani = nullptr;		///<ͬ����Ķ������
	Collider* col = nullptr;		///<ͬ�������ײ���

	action act;				///<ִ���¼�(ÿִ֡��) һ���û���д�½ڵ��������¼�
	action enter;			///<�����¼� һ���û���д�½ڵ��������¼�
	action exit;			///<�˳��¼� һ���û���д�½ڵ��������¼�
};

/**
 * FSM��Ҫ�Ĳ���.
 * 
 * ��Ҫ��Щ���Լӽ�ȥ
 */
struct FSM_Info
{
	Animator* ani = nullptr;		///<���������� Ĭ���ǰ󶨵�����Ĳ����� ����������Ⱦ

	//time_point<steady_clock> clock;		//��ʱ
	Collider* col = nullptr;		///<�󶨵���ײ��
	
};

/**
 * ״̬�����.
 * @brief ִ��״̬�Ĺ���
 */
class FSM final:public Component
{
	DECLARE_EDGE_CONDITION_BY_TYPE(FSMEdge, FSMItems)
public:
	
	/**
	 * ��ӽڵ�״̬ 
	 * @briefĬ��״̬����.
	 * @remark !!!�û���Ӵ����½ڵ�Ҫ����� 
	 * \param [in] name ״̬�ڵ�����
	 * \param [in] parent ���ڵ�
	 * \return �½ڵ��ָ��
	 */
	template<class T>
	T* AddStatus(std::string name, std::string parent = "root")
	{
		////����Ѿ������˵�ǰ�ڵ�
		//if (status.find(name) != nullptr)return nullptr;

		//Ѱ��״̬���Ƿ��и��ڵ㣬һ��Ҫ����һ�����ڵ������
		FSMItems* it = FindStatusByName(parent);
		if (!it) {
			Log::instance()->printf(Log::LEVEL_ERR, __FILE__, __LINE__,
				"���ڵ�:%s������", parent.c_str()); return nullptr;
		}	


		//�����ڵ�
		T* item = new T(it->newchildstatus(), it);
	
		if (item && static_cast<FSMItems*>(item))		//�þ�̬ת������Ϊ����ת��Ϊ���࣬���ܸ���ת��������
		{
			item->fsm = this;
			item->ani = this->fsm_info.ani;
			item->col = this->fsm_info.col;

			//ӳ��
			status.insert({ name,item });
			return item;
		}
		Log::instance()->printf(Log::LEVEL_ERR, __FILE__, __LINE__,
			"�����ڵ�״̬:%sʧ��", name.c_str());
		delete item;
		return nullptr;
	};

	/**
	 * �Ƴ��ڵ�״̬
	 * @brief Ĭ��ɾ�������ӽڵ�״̬.
	 * 
	 * \param [in] name ״̬��
	 * \param [in] delchild  �Ƿ�ɾ�������ӽڵ�
	 */
	void RemoveStatus(std::string name,bool delchild=1);
private:
	/**
	 * ע��״̬��״̬��.
	 * 
	 * \param [in] name ״̬��
	 * \param [in] item ״̬�ڵ�
	 */
	void RegisterStatus(std::string name,FSMItems* item)
	{
		status.insert({name,item});
	}

	/**
	 * �ݹ���ϵ���ִ����Ϊ.
	 * 
	 * \param [in] item ״̬�ڵ� 
	 */
	void process_Update(FSMItems* item);

	/**
	 * �ݹ���ϵ���ִ�н����¼�.
	 * 
	 * \param  [in] item ִ�нڵ�
	 * \param  [in]  end ���ĸ��ϲ�ڵ㿪ʼ
	 */
	void process_Enter(FSMItems* item, FSMItems* end = nullptr);

	/**
	 * �ݹ���µ���ִ���˳��¼�.
	 * 
	 * \param [in] item ִ�нڵ�
	 * \param [in]  end ִ�е��ĸ��ϲ�ڵ�Ϊֹ
	 */
	void process_Exit(FSMItems* item, FSMItems* end = nullptr);

	/**
	 * ÿһִ֡����Ϊ.
	 * @remark 1.���޹����������б� ����2.�Ѿ�������������ִ��һ����Ϊ����������һ���ڵ�
	 * \param [in] deltaTime ֡���ʱ��
	 */
	virtual void Update(float deltaTime);

	/**
	 * ���ĳ���ڵ���״̬(Ҳ������ǰ�ڵ�).
	 * 
	 * \param [in] name ״̬�ڵ���
	 */
	void Clear(std::string name);

	/**
	 * ȡ��ע��״̬��״̬��.
	 * 
	 * \param [in] name ״̬�ڵ���
	 */
	void UnregisterStatus(std::string name)
	{
		auto it = status.find(name);		
		status.erase(it);			
	}
	/**
	 * ȡ��ע��״̬��״̬��.
	 *
	 * \param [in] name ״̬�ڵ�
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
	 * �õ���ǰִ�е�״̬�ڵ�.
	 * 
	 * \return ״̬�ڵ�
	 */
	FSMItems* GetNormalStatus()
	{
		return normalstatus;
	}

	/**
	 * �õ���һ��ִ�е�״̬��.
	 * 
	 * \return ״̬��
	 */
	std::string GetLastStatus()
	{
		return laststatus;
	}

	/**
	 * ��ȡ״̬�ڵ�����.
	 * 
	 * \param [in] item ״̬�ڵ�
	 * \return ״̬����
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
	 * �����ֲ���״̬�ڵ�.
	 * 
	 * \param [in] name ״̬���� 
	 * \return ״̬�ڵ�
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
	 * ����һЩ����.
	 * @remark FSM_Info���������ЩҲҪ����
	 * @remark Ҫ��̬���ؾ�д��update����
	 */
	virtual void BeginPlay()
	{
		//������Ϣ
	fsm_info.ani = GetOwner()->GetComponentByClass<Animator>(); 
	fsm_info.col= GetOwner()->GetComponentByClass<Collider>();
	}

	////��״̬��������
	//std::string GetName()const { for (auto& it : status) { if (&(it.second) == (aninode))return it.first; }; }
private:
	/**
	 * Ѱ�������������.
	 * 
	 * \param [in] root ��״̬�ڵ� ���е�һ����������
	 * \param [in] p   ��һ��״̬�ڵ�
	 * \param [in] q   �ڶ���״̬�ڵ�
	 * \return  ����������Ƚڵ�
	 */
	FSMItems* lowestCommonAncestor(FSMItems* root, FSMItems* p, FSMItems* q);

public:
	/**
	 * ��ȡ�ڵ�·��.
	 * @brief �����������Ƚ��ȳ�
	 * \param [in] item ״̬�ڵ�
	 * \return  ·���ڵ����� �洢���Լ���ʼ�����ڵ� 
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
	 * �л�״̬.
	 * @remark �л�״̬���õĺ���
	 * @remark ���������˵�һ�ν���״̬���������ڳ�����ֱ��ʹ��
	 * @remark �ڵ��л��ȳ�ʼ�������� ���������ͬʱ��״̬�����ͬ�����ֵ�ʵ��
	 * @see CondiEdge
	 * \param [in] name ״̬��
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
	 * �л�״̬.
	 * @remark �л�״̬���õĺ���
	 * \param [in] item ״̬�ڵ�
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
	 * ��ʼ��.
	 * @brief ��Ҫ��Ϊ���Ժ���Ϊһ������������Ҫ�ĺ��� ʵ�ָ��Ӹ��ӵĹ���
	 */
	void Init() {};

	/**
	 * ����״̬��.
	 * 
	 */
	void Destruct();

	/**
	 * ����״̬��.
	 * ����ò���
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
	 * ���캯��.
	 * @brief��Ҫ���Ǵ������ڵ�
	 */
	FSM() 
	{
		
		root = new FSMItems(0); status.insert({ "root",root }); root->fsm = this; 
		  TransitionState("root");
	}

	/**
	 * ��������
	 * �������нڵ������Դ.
	 * 
	 */
	~FSM() { Destruct(); }

	/**
	 * ��Ӳ�����ʵ��ֵ������.
	 * @remark ֻ��������������������������û��޸�ֵ
	 * \param [in] paramName ��������
	* \param [in] type  ״̬��������
	*/
	void AddParamater(std::string paramName, ParamType type);

	/**
	 * ���û��޸Ĳ���.
	 *@remark Ҫ��AddParamater�ٵ������
	 * \param [in] paramName ��������
	 * \param [in] value T������ֵ
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
		// �����������͵����
		Log::instance()->printf(Log::LEVEL_WAR, __FILE__, __LINE__,
			"Unsupported type for parameter: %s", paramName);
	};

	/**
	 * ����������.
	 *
	 * \param [in] paramName ��������
	 */
	void SetTrigger(std::string paramName);

	/**
	 * �Ƿ���Ϲ�������.
	 *
	 * \param edge ����������
	 * \return    true�������� false����������
	 */
	bool CheckConditions(FSMEdge* edge);

private:
	/**
	 * �ڵ�ӳ�� [���֣�״̬�ڵ�]
	 * �����Ʒ��ʽڵ�.
	 */
	std::unordered_map<std::string, FSMItems*> status;
	///���ڵ�
	FSMItems* root ;
	///����״̬�ڵ�
	FSMItems* normalstatus=nullptr;

	/**
	 * ǰһ״̬ 
	 * @remark û����Ϊnull ����ǰһ��ɾ����.
	 */
	std::string laststatus = "null";

	/**
	 * ��Ҫ�Ĳ���.
	 */
	FSM_Info fsm_info;

	//�������ʵ��
	std::unordered_map<std::string, int> parametersInteger;				///<ʵ��������ֵ�������� [�����������֣�ʵ����ֵ]
	std::unordered_map<std::string, float> parametersFloat;				///<ʵ�ʸ�������ֵ�������� [�����������֣�ʵ����ֵ]		
	std::unordered_map<std::string, bool> parametersBool;				///<ʵ�ʲ�������ֵ�������� [�����������֣�ʵ����ֵ]
	std::unordered_map<std::string, bool> parametersTrigger;			///<ʵ�ʴ�������ֵ�������� [�����������֣�ʵ����ֵ]
};