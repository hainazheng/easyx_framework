/*****************************************************************//**
 * \file   CoreMinimal.h
 * \brief  核心代码文件
 *  @remark 包含框架基本头文件以及基础类、全局函数
 * \author zhn
 *********************************************************************/

#pragma once
#include<cmath>
#include<iostream>
#include <set>
#include<unordered_set>
#include<string>
#include<vector>
#include <mutex>
#include<thread>
#include"Macro.h"
#include"Struct.h"

//constexpr double DELTA_TIME = 0.01;		//每一帧的时间是0.01秒 手动限制

/**
 * 基类.
 * @brief 框架大部分类的基类
 * @see 只有继承他才有一些基本虚函数，可以作为框架的一部分进行使用
 */
class base
{
	static int tag;			///<整个框架的标识

	std::string name;			///<类似"class base * __ptr64 tag的值"
public:
	base() { tag++; name = typeid(this).name() + std::string(" ") + std::to_string(tag); }
	virtual ~base() { tag--; };
	//获取标签名
	std::string GetName()const { return name; }
	//设置标签名
	void SetName(const std::string& name) { this->name = name; }
	virtual void Update(float deltaTime) {};		//纯虚函数  所有的类都希望统一用update执行操作
	virtual void BeginPlay() = 0;
	virtual void EndPlay() = 0;
};

///全局游戏实例
class GameInstance
{
	//class FSM* game_status=nullptr;
};

///全局游戏关卡类
class Level :public base
{
	friend class World;
	friend class GameStatics;
protected:
	class Controller* mainController = nullptr;			///控制者
public:
	virtual void Update(float deltaTime)override
	{
	}

	/**
	 * 务必把关卡初始化逻辑写进BeginPlay，禁止写入构造函数.
	 */
	virtual void BeginPlay() {};
	virtual void EndPlay() {};
};


///类型转化
///@remark 只要是本实参类型的基类及其派生类类型，就进行转化成想要转化成的它能转化的类型
template<class T, class F>
inline T* Cast(F* pBase)
{
	if (pBase)return dynamic_cast<T*>(pBase);
	std::cout << "Cast error" << std::endl;
	return nullptr;
}

/**
 * 获取年月日时分秒字符串.
 * - 1 - 年
 * - 2 - 月
 * - 3 - 日
 * - 4 - 时
 * - 5 - 分
 * - 6 - 秒
 * - default 错误
 * \param [in] pre 代表获取的最低精度 
 * \return 
 */
inline std::string GetRealTime(int pre = 3)
{
	std::time_t now_time_t = std::time(nullptr);

	// 定义一个tm结构体来存储本地时间
	std::tm now_tm;

	// 使用localtime_s获取本地时间
	if (localtime_s(&now_tm, &now_time_t) == 0) {
		// 获取年、月、日、时
		int year = now_tm.tm_year + 1900; // tm_year是从1900年开始的年数
		int month = now_tm.tm_mon + 1;    // tm_mon是从0开始的月份
		int day = now_tm.tm_mday;
		int hour = now_tm.tm_hour;
		int min = now_tm.tm_min;
		int sec = now_tm.tm_sec;

		std::string real_tm = std::to_string(year);
		switch (pre)
		{
		case 2:
			real_tm = real_tm + "-" + std::to_string(month); break;
		case 3:
			real_tm = real_tm + "-" + std::to_string(month) + "-"
				+ std::to_string(day); break;
		case 4:
			real_tm = real_tm + "-" + std::to_string(month) + "-"
				+ std::to_string(day) + "-" + std::to_string(hour); break;
		case 5:
			real_tm = real_tm + "-" + std::to_string(month) + "-"
				+ std::to_string(day) + "-" + std::to_string(hour) + ":" + std::to_string(min); break;
		case 6:
			real_tm = real_tm + "-" + std::to_string(month) + "-"
				+ std::to_string(day) + "-" + std::to_string(hour) + ":" + std::to_string(min) + ":" + std::to_string(sec); break;
		default:
			break;
		}
		return real_tm;
	}
	else std::cerr << "Failed to get local time." << std::endl;
	return std::string{};
}

///是否开启帧率限制
/// @remark !!! 一般用户都要进行开启 否则不稳定
extern bool bFrameLimitOn;

///最大帧率(每秒执行多少次)
///  @remark !!! 一般用户都要进行设置 但是太大每一帧时间太短会极大影响框架
extern int MaxFrame;

