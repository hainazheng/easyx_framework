/*****************************************************************//**
 * \file   CoreMinimal.h
 * \brief  ���Ĵ����ļ�
 *  @remark ������ܻ���ͷ�ļ��Լ������ࡢȫ�ֺ���
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

//constexpr double DELTA_TIME = 0.01;		//ÿһ֡��ʱ����0.01�� �ֶ�����

/**
 * ����.
 * @brief ��ܴ󲿷���Ļ���
 * @see ֻ�м̳�������һЩ�����麯����������Ϊ��ܵ�һ���ֽ���ʹ��
 */
class base
{
	static int tag;			///<������ܵı�ʶ

	std::string name;			///<����"class base * __ptr64 tag��ֵ"
public:
	base() { tag++; name = typeid(this).name() + std::string(" ") + std::to_string(tag); }
	virtual ~base() { tag--; };
	//��ȡ��ǩ��
	std::string GetName()const { return name; }
	//���ñ�ǩ��
	void SetName(const std::string& name) { this->name = name; }
	virtual void Update(float deltaTime) {};		//���麯��  ���е��඼ϣ��ͳһ��updateִ�в���
	virtual void BeginPlay() = 0;
	virtual void EndPlay() = 0;
};

///ȫ����Ϸʵ��
class GameInstance
{
	//class FSM* game_status=nullptr;
};

///ȫ����Ϸ�ؿ���
class Level :public base
{
	friend class World;
	friend class GameStatics;
protected:
	class Controller* mainController = nullptr;			///������
public:
	virtual void Update(float deltaTime)override
	{
	}

	/**
	 * ��ذѹؿ���ʼ���߼�д��BeginPlay����ֹд�빹�캯��.
	 */
	virtual void BeginPlay() {};
	virtual void EndPlay() {};
};


///����ת��
///@remark ֻҪ�Ǳ�ʵ�����͵Ļ��༰�����������ͣ��ͽ���ת������Ҫת���ɵ�����ת��������
template<class T, class F>
inline T* Cast(F* pBase)
{
	if (pBase)return dynamic_cast<T*>(pBase);
	std::cout << "Cast error" << std::endl;
	return nullptr;
}

/**
 * ��ȡ������ʱ�����ַ���.
 * - 1 - ��
 * - 2 - ��
 * - 3 - ��
 * - 4 - ʱ
 * - 5 - ��
 * - 6 - ��
 * - default ����
 * \param [in] pre �����ȡ����;��� 
 * \return 
 */
inline std::string GetRealTime(int pre = 3)
{
	std::time_t now_time_t = std::time(nullptr);

	// ����һ��tm�ṹ�����洢����ʱ��
	std::tm now_tm;

	// ʹ��localtime_s��ȡ����ʱ��
	if (localtime_s(&now_tm, &now_time_t) == 0) {
		// ��ȡ�ꡢ�¡��ա�ʱ
		int year = now_tm.tm_year + 1900; // tm_year�Ǵ�1900�꿪ʼ������
		int month = now_tm.tm_mon + 1;    // tm_mon�Ǵ�0��ʼ���·�
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

///�Ƿ���֡������
/// @remark !!! һ���û���Ҫ���п��� �����ȶ�
extern bool bFrameLimitOn;

///���֡��(ÿ��ִ�ж��ٴ�)
///  @remark !!! һ���û���Ҫ�������� ����̫��ÿһ֡ʱ��̫�̻Ἣ��Ӱ����
extern int MaxFrame;

