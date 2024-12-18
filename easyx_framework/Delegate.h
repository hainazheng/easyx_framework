/*****************************************************************//**
 * \file   Delegate.h
 * \brief  ί����.
 * ʵ�ֵ���ί�кͶಥί�У���һ��ͳһ�Ľӿ�ʵ�ֺ������ѿ���Ȩ����ί����
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include<functional>
#include<vector>



/**
 * ����ί��.
 * UniCastDelegate<����ֵ���ͣ���������...>
 * @see DECLARE_MULTI_PARAM_WITH_RETURN_UNICAST_DELEGATE(name,R,...)
 * @see DECLARE_NO_PARAM_WITH_RETURN_UNICAST_DELEGATE(name,R)
 * @see DECLARE_MULTI_PARAM_UNICAST_DELEGATE(name,...) 
 * @see DECLARE_NO_PARAM_UNICAST_DELEGATE(name
 */
template<typename R=void,typename... Args>			//�ɱ����ģ�� �����ͽ�����չ
class UniCastDelegate
{
	std::function<R(Args...)>callback;			///<����Args...�����Ͳ����б������չ �����ʱ��
public:
	///���а�
	void Bind(
		std::function<R(Args...)> callback  ///<[in][lambda����]
	) { this->callback = callback; }
	
	//���г�Ա������ [�������󣬺���ָ��]
	template<typename T>
	void Bind(
		T* obj				///<[in]��������
		,R(T::* func)(Args...)		///<[in]����ָ��
	)
	{
		std::function<R(Args...)> callback = [=](Args... args) {(obj->*func)(args...); };			//args��չ�����б� �ڲ����õ�ʱ��
		Bind(callback);
	}

	///�����
	void UnBind() { callback = nullptr; }

	///�Ƿ��а󶨵ĺ���
	bool IsBinded()const { return callback != nullptr; }

	R Execute(
		Args... args		///<[in]��Ҫ�Ĳ���
	);

	R operator()(
		Args... args		///<[in]��Ҫ�Ĳ���
		) { return Execute(args...); }
};

/**
 * �ಥί��.
 * MultiCastDelegate<����ֵ���ͣ���������...>
 * @see DECLARE_MULTI_PARAM_MULTI_DELEGATE_CLASS(name, ...)
 * @see DECLARE_NO_PARAM_MULTI_DELEGATE_CLASS(name)
 */
template<typename... Args>
class MultiCastDelegate
{
	std::vector<std::function<void(Args...)>> callbacks;		///<�洢������ͬ���Ͳ����ĺ���
public:
	///��������
	void Push(
		std::function<void(Args...)			///[in] [lambda����]
	> callback);

	template<typename T>
	void Push(
		T* obj					///<[in]��������
		,void(T::* func)(Args...)		///<[in]����ָ��
	)
	{
		std::function<void(Args...)> callback = [=](Args... args) { (obj->*func)(args...); };
		Push(callback);
	}

	///���������Ƴ�������ͬ�ĺ���
	void Remove(
		std::function<void(Args...)> callback	///[in] [lambda����]
	);

	///���������Ƴ�������ͬ�ĺ���
	template<typename T>
	void Remove(
		T* obj							///<[in] ��������
		, void(T::* func)(Args...)		///<[in]����ָ��
	)
	{
		std::function<void(Args...)> callback = [=](Args... args) { (obj->*func)(args...); };
		Remove(callback);
	}
	
	///ȫ��ִ��
	void BroadCast(
		Args ...args	///<[in] ����ִ����Ҫ��ʵ��
	);

	void operator()(Args... args){ this->BroadCast(args...); }
};

///@remark �����ĺ궨��ʵ������ �з���ֵ
#define DECLARE_MULTI_PARAM_WITH_RETURN_UNICAST_DELEGATE(name,R,...) class name:public UniCastDelegate<R,__VA_ARGS__>{};
///@remark �����ĺ궨��ʵ������ �з���ֵ û�в���
#define DECLARE_NO_PARAM_WITH_RETURN_UNICAST_DELEGATE(name,R) class name:public UniCastDelegate<R>{};

///@remark �����ĺ궨��ʵ������ û�з���ֵ
#define DECLARE_MULTI_PARAM_UNICAST_DELEGATE(name,...) class name:public UniCastDelegate<__VA_ARGS__>{};
///@remark �����ĺ궨��ʵ������ û�з���ֵ û�в���
#define DECLARE_NO_PARAM_UNICAST_DELEGATE(name) class name:public UniCastDelegate<>{};

///@remark �ಥ�ĺ궨��ʵ������ 
#define DECLARE_MULTI_PARAM_MULTI_DELEGATE_CLASS(name, ...) class name:public MultiCastDelegate<__VA_ARGS__>{};		//c++֧�ֵĺ�ɱ䳤ռλ��__VA_ARGS__
///@remark �ಥ�ĺ궨��ʵ������ û�в���
#define DECLARE_NO_PARAM_MULTI_DELEGATE_CLASS(name) class name:public MultiCastDelegate<>{};

///����ִ��
template<typename R, typename ...Args>
inline R UniCastDelegate<R, Args...>::Execute(Args... args)
{
	if (callback)
	{
		callback(args...);
	}
	return R();
}

///�������ͬ�����͵��¼�
template< typename ...Args>
inline void MultiCastDelegate< Args...>::Push(std::function<void(Args...)> callback)
{
	for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
	{
		//if (it->target<void(Args...)>() == callback.target<void(Args...)>())			//��ȡĳ�������ĵ�ַ ��ַ��ͬ˵�����Ѿ�����
		//{
		//	std::cout << "ί���ظ�" << std::endl;
		//	return;
		//}
	}
	callbacks.push_back(callback);
}

template<typename ...Args>
inline void MultiCastDelegate<Args...>::Remove(std::function<void(Args...)> callback)
{
	for (auto it = callbacks.begin(); it != callbacks.end(); )
	{
		if (it->target<void(Args...)>() == callback.target<void(Args...)>())			//��ȡĳ�������ĵ�ַ��ַ��ͬ˵�����Ѿ�����
		{

			it = callbacks.erase(it);			//��������������Ҫ���ظ��������������Ϊ�� �����Ϊ�գ��Ͳ��ܽ��в��� ������Ϊbegin���ݼ������

		}
		else
			it++;
	}
	//callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
}

template<typename ...Args>
inline void MultiCastDelegate<Args...>::BroadCast(Args ...args)
{

		for (auto& callback : callbacks)
		{
			/*std::cout << "ִ�гɹ�" << std::endl;*/
			callback(args...);
		}
	
}
