/*****************************************************************//**
 * \file   Math.h
 * \brief  全局数学函数库定义
 * 
 * \author zhn
 *********************************************************************/

#pragma once
#include<cmath>


#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

///全局数学函数库
class Math
{

public:
	static int RandInt(
		int lower		///<[in]区间最小值
		, int upper		///<[in]区间最大值
	);

	static double RandReal(
		double lower	///<[in]区间最小值
		, double upper	///<[in]区间最大值
	);

	static double RandPerc();

	///将某数限制在某个范围内
	template<typename T>
	static T Clamp(
		T value		///<[in]某个数
		, T lower	///<[in]区间最小值
		, T upper	///<[in]区间最大值
	)
	{
		return min(max(value, min(lower, upper)), max(lower, upper));
	}
	static double Clamp(double value, double lower, double upper);

	///映射函数
	/// 是不是闭区间要看旧区间是不是闭区间
	/// @remark ！！！全部参数都是double类型计算量可能会很大，性能开销要注意
	static double mapping(
		double value				///<[in]某个数
		, double oldlower			///<[in]旧区间最小值
		, double oldupper			///<[in]旧区间最大值
		, double newlower			///<[in]新区间最小值
		, double newupper			///<[in]新区间最大值
	);
};


