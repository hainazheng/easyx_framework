/*****************************************************************//**
 * \file   Timer.h
 * \brief  ��ʱ����
 * \brief  ��������м̳�
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

///��ʱ����
///@remark��������м̳�
class Timer final
{
public:
	///@remark ʵ�ֿ��Բ��ð�Ҳ���Ի�ȡʱ��
	Timer() { lastTime = steady_clock::now(); };			
	///@remark һ�㶼�Ǵ����ֲ���ʱ�������ö�̬ɾ����ֻҪ��ȫ��������ɾȥ����
	~Timer() { mainWorld.GameTimers.erase(this); };			

	///ʵ�ָ����������ʵ�ּ�ʱ  
	/// @remark Ĭ�ϲ��ظ�ִ��,����ָ���״�ִ��ʱ������Ĭ��Ϊû�У�
	template<class T>
	void Bind(
		double del							///<[in]��ʱ
		,T*obj								///<[in]Ҫ�󶨵Ķ���
		,void(T::*func)()					///<[in]Ҫִ�еĺ���,ֻ����û�з���ֵ��void�ĺ��������ߺ���ָ��
		,bool repeat =false					///<[in]�Ƿ�Ҫ�ظ�ִ��
		,double firstDelay=-1.0	
	)	
	{
		callback = std::bind(func, obj); 		//��bind��װ���ʶ���ĳ�Ա��������Ҫ���Ķ���ͺ���ָ��
		bpersistent = repeat;
		delay = duration<double>(del);
		lastTime = steady_clock::now();
		if (firstDelay >= 0)lastTime -= milliseconds(int(1000*(del-firstDelay)));			//��ǰʱ�仹Ҫ��ȥ������״�ִ�еĲ�ֵ�൱���Ѿ�ִ���˶���ʱ��
		mainWorld.GameTimers.insert(this);			//�������������
	}
	///ʹ��lambda���ʽ���к�����װ
	void Bind(
		double del							///<[in]��ʱ
		, std::function<void()>fun			///<[in]lambda���ʽ
		, bool repeat = false				///<[in]�Ƿ�Ҫ�ظ�ִ��
		,double firstDelay = -1.0			///<[in]�״�ִ��ʱ��
	)			
	{
		callback = fun;			//��bind��װ���ʶ���ĳ�Ա��������Ҫ���Ķ���ͺ���ָ��
		bpersistent = repeat;
		delay = duration<double>(del);
		lastTime = steady_clock::now();
		if (firstDelay >= 0)lastTime -= milliseconds(int(1000 * (del - firstDelay)));
		mainWorld.GameTimers.insert(this);			//�������������
	}

	///ִ�м�ʱ��
	void Execute()			
	{
		if (delay.count() > 0 && getDelay().count() >= delay.count() && bRunning)				//������ʱ����Ϊ0������ʱ�䲻��С�����õ���ʱʱ��
		{
			callback();
			if (bpersistent) lastTime = steady_clock::now();							//���Ҫ�ظ�ִ�У�������ϴε�ʱ���
			else
			{
				delay = duration<double>(0);
			}
		}
	}

	///��ȡ���ʱ��
	double GetDelay() { return getDelay().count(); };

	///���ü��ʱ��
	void SetDelay(
		double time		///<[in]�������
	) { delay = duration<double>(time); };

	///����ʱ��㣬���¿�ʼ��ʱ
	void ReSet() { lastTime = steady_clock::now(); };

	///ֹͣ��ʱ��  Ҫ��¼ͣ�ٵ�ʱ����
	void Stop() { if (!bRunning)return; bRunning = false; stopTime = steady_clock::now(); };

	///������ʱ�� 
	/// @remark ʱ����Ҫ�����ϴ�ִ�е�ʱ��������ȷ��������Ҫ���¼����
	void Continue()
	{
		//��ֹ�ظ�����lastTime
		if (bRunning)return;
		bRunning = true; lastTime += (steady_clock::now() - stopTime);
	}

private:
	std::function<void()>callback;		///<�Ѻ���ָ��������䣬Ҳ���ǰ�װ
	bool bpersistent = false;			///<�Ƿ��ظ�ִ��
	bool bRunning = true;				///<�Ƿ����ڼ�ʱ
	
	duration<double> delay = duration<double>(0);			///<ʱ������� ������ʱʱ�����������ʱ��β���ִ�� Ĭ��Ϊ�� ʵ�ּ�ʱ����
	time_point<steady_clock> lastTime;						///<ʱ������Ͱ���һ�ε�ʱ��㴫��ȥ ʱ�ӽ��������뼶��
	time_point<steady_clock> stopTime;						///<ֹͣ��ʱ���
	
	/// ///��ȡ��ʱʱ��
	duration<double> getDelay()
	{
		return steady_clock::now() - lastTime;					//������ʱʱ��
	}
};