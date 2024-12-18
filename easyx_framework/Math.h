/*****************************************************************//**
 * \file   Math.h
 * \brief  ȫ����ѧ�����ⶨ��
 * 
 * \author zhn
 *********************************************************************/

#pragma once
#include<cmath>


#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

///ȫ����ѧ������
class Math
{

public:
	static int RandInt(
		int lower		///<[in]������Сֵ
		, int upper		///<[in]�������ֵ
	);

	static double RandReal(
		double lower	///<[in]������Сֵ
		, double upper	///<[in]�������ֵ
	);

	static double RandPerc();

	///��ĳ��������ĳ����Χ��
	template<typename T>
	static T Clamp(
		T value		///<[in]ĳ����
		, T lower	///<[in]������Сֵ
		, T upper	///<[in]�������ֵ
	)
	{
		return min(max(value, min(lower, upper)), max(lower, upper));
	}
	static double Clamp(double value, double lower, double upper);

	///ӳ�亯��
	/// �ǲ��Ǳ�����Ҫ���������ǲ��Ǳ�����
	/// @remark ������ȫ����������double���ͼ��������ܻ�ܴ����ܿ���Ҫע��
	static double mapping(
		double value				///<[in]ĳ����
		, double oldlower			///<[in]��������Сֵ
		, double oldupper			///<[in]���������ֵ
		, double newlower			///<[in]��������Сֵ
		, double newupper			///<[in]���������ֵ
	);
};


