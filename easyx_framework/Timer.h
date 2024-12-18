/*****************************************************************//**
 * \file   Timer.h
 * \brief  计时器类
 * \brief  不允许进行继承
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"
#include<functional>
#include<chrono>
#include"World.h"
using std::chrono::steady_clock;
using std::chrono::duration;
using std::chrono::time_point;
using std::chrono::milliseconds;

///计时器类
///@remark不允许进行继承
class Timer final
{
public:
	///@remark 实现可以不用绑定也可以获取时间
	Timer() { lastTime = steady_clock::now(); };			
	///@remark 一般都是创建局部计时器，不用动态删除，只要从全局容器中删去就行
	~Timer() { mainWorld.GameTimers.erase(this); };			

	///实现跟对象物体绑定实现计时  
	/// @remark 默认不重复执行,可以指定首次执行时间间隔（默认为没有）
	template<class T>
	void Bind(
		double del							///<[in]延时
		,T*obj								///<[in]要绑定的对象
		,void(T::*func)()					///<[in]要执行的函数,只能是没有返回值的void的函数名或者函数指针
		,bool repeat =false					///<[in]是否要重复执行
		,double firstDelay=-1.0	
	)	
	{
		callback = std::bind(func, obj); 		//用bind包装访问对象的成员函数，需要他的对象和函数指针
		bpersistent = repeat;
		delay = duration<double>(del);
		lastTime = steady_clock::now();
		if (firstDelay >= 0)lastTime -= milliseconds(int(1000*(del-firstDelay)));			//当前时间还要减去间隔和首次执行的差值相当于已经执行了多少时间
		mainWorld.GameTimers.insert(this);			//插入世界的容器
	}
	///使用lambda表达式进行函数包装
	void Bind(
		double del							///<[in]延时
		, std::function<void()>fun			///<[in]lambda表达式
		, bool repeat = false				///<[in]是否要重复执行
		,double firstDelay = -1.0			///<[in]首次执行时间
	)			
	{
		callback = fun;			//用bind包装访问对象的成员函数，需要他的对象和函数指针
		bpersistent = repeat;
		delay = duration<double>(del);
		lastTime = steady_clock::now();
		if (firstDelay >= 0)lastTime -= milliseconds(int(1000 * (del - firstDelay)));
		mainWorld.GameTimers.insert(this);			//插入世界的容器
	}

	///执行计时器
	void Execute()			
	{
		if (delay.count() > 0 && getDelay().count() >= delay.count() && bRunning)				//设置延时不能为0，经过时间不能小于设置的延时时间
		{
			callback();
			if (bpersistent) lastTime = steady_clock::now();							//如果要重复执行，则更新上次的时间点
			else
			{
				delay = duration<double>(0);
			}
		}
	}

	///获取间隔时间
	double GetDelay() { return getDelay().count(); };

	///设置间隔时间
	void SetDelay(
		double time		///<[in]间隔秒数
	) { delay = duration<double>(time); };

	///重置时间点，重新开始计时
	void ReSet() { lastTime = steady_clock::now(); };

	///停止计时器  要记录停顿的时间间隔
	void Stop() { if (!bRunning)return; bRunning = false; stopTime = steady_clock::now(); };

	///继续计时器 
	/// @remark 时间间隔要加上上次执行的时间点才是正确的我们需要的事件间隔
	void Continue()
	{
		//防止重复更新lastTime
		if (bRunning)return;
		bRunning = true; lastTime += (steady_clock::now() - stopTime);
	}

private:
	std::function<void()>callback;		///<把函数指针进行适配，也就是包装
	bool bpersistent = false;			///<是否重复执行
	bool bRunning = true;				///<是否正在计时
	
	duration<double> delay = duration<double>(0);			///<时间段类型 设置延时时间间隔超过这个时间段才能执行 默认为秒 实现计时控制
	time_point<steady_clock> lastTime;						///<时间点类型把上一次的时间点传进去 时钟进度在纳秒级别
	time_point<steady_clock> stopTime;						///<停止的时间点
	
	/// ///获取延时时间
	duration<double> getDelay()
	{
		return steady_clock::now() - lastTime;					//返回延时时间
	}
};