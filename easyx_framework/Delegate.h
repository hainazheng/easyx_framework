/*****************************************************************//**
 * \file   Delegate.h
 * \brief  委托类.
 * 实现单播委托和多播委托，用一个统一的接口实现函数，把控制权交给委托类
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include<functional>
#include<vector>



/**
 * 单播委托.
 * UniCastDelegate<返回值类型，参数类型...>
 * @see DECLARE_MULTI_PARAM_WITH_RETURN_UNICAST_DELEGATE(name,R,...)
 * @see DECLARE_NO_PARAM_WITH_RETURN_UNICAST_DELEGATE(name,R)
 * @see DECLARE_MULTI_PARAM_UNICAST_DELEGATE(name,...) 
 * @see DECLARE_NO_PARAM_UNICAST_DELEGATE(name
 */
template<typename R=void,typename... Args>			//可变参数模板 对类型进行扩展
class UniCastDelegate
{
	std::function<R(Args...)>callback;			///<这里Args...对类型参数列表进行扩展 定义的时候
public:
	///进行绑定
	void Bind(
		std::function<R(Args...)> callback  ///<[in][lambda函数]
	) { this->callback = callback; }
	
	//进行成员函数绑定 [函数对象，函数指针]
	template<typename T>
	void Bind(
		T* obj				///<[in]函数对象
		,R(T::* func)(Args...)		///<[in]函数指针
	)
	{
		std::function<R(Args...)> callback = [=](Args... args) {(obj->*func)(args...); };			//args扩展参数列表 内部调用的时候
		Bind(callback);
	}

	///解除绑定
	void UnBind() { callback = nullptr; }

	///是否有绑定的函数
	bool IsBinded()const { return callback != nullptr; }

	R Execute(
		Args... args		///<[in]需要的参数
	);

	R operator()(
		Args... args		///<[in]需要的参数
		) { return Execute(args...); }
};

/**
 * 多播委托.
 * MultiCastDelegate<返回值类型，参数类型...>
 * @see DECLARE_MULTI_PARAM_MULTI_DELEGATE_CLASS(name, ...)
 * @see DECLARE_NO_PARAM_MULTI_DELEGATE_CLASS(name)
 */
template<typename... Args>
class MultiCastDelegate
{
	std::vector<std::function<void(Args...)>> callbacks;		///<存储所有相同类型参数的函数
public:
	///推入容器
	void Push(
		std::function<void(Args...)			///[in] [lambda函数]
	> callback);

	template<typename T>
	void Push(
		T* obj					///<[in]函数对象
		,void(T::* func)(Args...)		///<[in]函数指针
	)
	{
		std::function<void(Args...)> callback = [=](Args... args) { (obj->*func)(args...); };
		Push(callback);
	}

	///从容器中移除所有相同的函数
	void Remove(
		std::function<void(Args...)> callback	///[in] [lambda函数]
	);

	///从容器中移除所有相同的函数
	template<typename T>
	void Remove(
		T* obj							///<[in] 函数对象
		, void(T::* func)(Args...)		///<[in]函数指针
	)
	{
		std::function<void(Args...)> callback = [=](Args... args) { (obj->*func)(args...); };
		Remove(callback);
	}
	
	///全部执行
	void BroadCast(
		Args ...args	///<[in] 函数执行需要的实参
	);

	void operator()(Args... args){ this->BroadCast(args...); }
};

///@remark 单播的宏定义实例化类 有返回值
#define DECLARE_MULTI_PARAM_WITH_RETURN_UNICAST_DELEGATE(name,R,...) class name:public UniCastDelegate<R,__VA_ARGS__>{};
///@remark 单播的宏定义实例化类 有返回值 没有参数
#define DECLARE_NO_PARAM_WITH_RETURN_UNICAST_DELEGATE(name,R) class name:public UniCastDelegate<R>{};

///@remark 单播的宏定义实例化类 没有返回值
#define DECLARE_MULTI_PARAM_UNICAST_DELEGATE(name,...) class name:public UniCastDelegate<__VA_ARGS__>{};
///@remark 单播的宏定义实例化类 没有返回值 没有参数
#define DECLARE_NO_PARAM_UNICAST_DELEGATE(name) class name:public UniCastDelegate<>{};

///@remark 多播的宏定义实例化类 
#define DECLARE_MULTI_PARAM_MULTI_DELEGATE_CLASS(name, ...) class name:public MultiCastDelegate<__VA_ARGS__>{};		//c++支持的宏可变长占位符__VA_ARGS__
///@remark 多播的宏定义实例化类 没有参数
#define DECLARE_NO_PARAM_MULTI_DELEGATE_CLASS(name) class name:public MultiCastDelegate<>{};

///单播执行
template<typename R, typename ...Args>
inline R UniCastDelegate<R, Args...>::Execute(Args... args)
{
	if (callback)
	{
		callback(args...);
	}
	return R();
}

///允许插入同样类型的事件
template< typename ...Args>
inline void MultiCastDelegate< Args...>::Push(std::function<void(Args...)> callback)
{
	for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
	{
		//if (it->target<void(Args...)>() == callback.target<void(Args...)>())			//获取某个函数的地址 地址相同说明是已经存在
		//{
		//	std::cout << "委托重复" << std::endl;
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
		if (it->target<void(Args...)>() == callback.target<void(Args...)>())			//获取某个函数的地址地址相同说明是已经存在
		{

			it = callbacks.erase(it);			//迭代器容器遍历要返回给迭代器，否则会为空 如果是为空，就不能进行操作 迭代器为begin，递减会出错

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
			/*std::cout << "执行成功" << std::endl;*/
			callback(args...);
		}
	
}
