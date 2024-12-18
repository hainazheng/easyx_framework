/*****************************************************************//**
 * \file   Component.h
 * \brief  �������
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"

/**
 * �������.
 */
class Component :public base
{

protected:
	class Object* pOwner = nullptr;		///<��������߼�  �ж��������ָ����ܽ��п���

	bool bIsEnabled = true;				///<�Ƿ񼤻�
public:
	virtual void Update(float deltaTime)override {};

	/**
	 * ���ô��麯�����Ѳ���д�ڹ���ȴ��Ҫ����ʵ�ֵ�д����.
	 */
	virtual void BeginPlay() {};

	///��һЩ����д������������һЩ����д������
	virtual void EndPlay() {};

	/**
	 * ���ð󶨶���.
	 * 
	 * @param �󶨶���
	 */
	void SetOwner(class Object* owner) { pOwner = owner; }

	///����������������������
	virtual void Destruct();

	///�������
	virtual void Activate();

	///ͣ�����
	virtual void Deactivate();

	///��ȡ�󶨶���ָ��
	Object* GetOwner() const
	{
		return pOwner;
	}
};