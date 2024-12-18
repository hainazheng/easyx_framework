/*****************************************************************//**
 * \file   UserInterface.h
 * \brief  UI类模板，用于容纳并管理ui小部件
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"widgets.h"
#include"easyx.h"

/**
 * UI类.
 * @remark 坐标都是相对的 要关注坐标系、坐标和大小 这样可以灵活运用提供的偏移找到需要的位置
 * @remark 关系不一定是树结构，也可以是图结构
 * @brief UI小部件 按照自己喜好可以自己创建 只要按照一定规则(继承Widget)
 * @see Widget
 */
class UserInterface :public base
{
	friend class Panel;
	std::unordered_set<Widget*> widgets;		///<绑定的UI部件
protected:
	Widget* const root = new Widget;			///<绑定的根小部件
public:
	UserInterface();
	~UserInterface() { for (auto& widget : widgets)delete widget; };

	virtual void Update(float deltaTime);
	virtual void BeginPlay() {};
	virtual void EndPlay() {};
	void ShowInfoBox() { for (auto& widget : widgets) { widget->ShowInfoBox(); } }
	void DrawDebugRect();

	/**
	 * 添加widget小部件到UI模板管理
	 * 创建小部件，注册到对象容器.
	 * @remark ！！！创建UI小部件只能用这个创建
	 * @see GameStatics::CreateUI()
	 * \return 创建成功返回创建成功的指针失败就返回nullptr
	 */
	template<class T>
	T* AddWidget()
	{
		T* Wid = new T;
		if (Wid && static_cast<Widget*>(Wid))		//用静态转化是因为子类转化为父类，不能父类转化成子类
		{
			widgets.insert(Wid);
			return Wid;
		}
		delete Wid;
		return nullptr;
	}

	///将UI添加到视口
	void AddToViewport();

	//将UI从视口暂时隐藏
	void HideFromViewport();

	//将UI从视口移除
	void RemoveFromViewport();

};



