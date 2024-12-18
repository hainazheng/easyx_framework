/*****************************************************************//**
 * \file   UserInterface.h
 * \brief  UI��ģ�壬�������ɲ�����uiС����
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"widgets.h"
#include"easyx.h"

/**
 * UI��.
 * @remark ���궼����Ե� Ҫ��ע����ϵ������ʹ�С ����������������ṩ��ƫ���ҵ���Ҫ��λ��
 * @remark ��ϵ��һ�������ṹ��Ҳ������ͼ�ṹ
 * @brief UIС���� �����Լ�ϲ�ÿ����Լ����� ֻҪ����һ������(�̳�Widget)
 * @see Widget
 */
class UserInterface :public base
{
	friend class Panel;
	std::unordered_set<Widget*> widgets;		///<�󶨵�UI����
protected:
	Widget* const root = new Widget;			///<�󶨵ĸ�С����
public:
	UserInterface();
	~UserInterface() { for (auto& widget : widgets)delete widget; };

	virtual void Update(float deltaTime);
	virtual void BeginPlay() {};
	virtual void EndPlay() {};
	void ShowInfoBox() { for (auto& widget : widgets) { widget->ShowInfoBox(); } }
	void DrawDebugRect();

	/**
	 * ���widgetС������UIģ�����
	 * ����С������ע�ᵽ��������.
	 * @remark ����������UIС����ֻ�����������
	 * @see GameStatics::CreateUI()
	 * \return �����ɹ����ش����ɹ���ָ��ʧ�ܾͷ���nullptr
	 */
	template<class T>
	T* AddWidget()
	{
		T* Wid = new T;
		if (Wid && static_cast<Widget*>(Wid))		//�þ�̬ת������Ϊ����ת��Ϊ���࣬���ܸ���ת��������
		{
			widgets.insert(Wid);
			return Wid;
		}
		delete Wid;
		return nullptr;
	}

	///��UI��ӵ��ӿ�
	void AddToViewport();

	//��UI���ӿ���ʱ����
	void HideFromViewport();

	//��UI���ӿ��Ƴ�
	void RemoveFromViewport();

};



