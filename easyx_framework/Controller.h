/*****************************************************************//**
 * \file   Controller.h
 * \brief  玩家控制器类定义
 * @remark（不唯一，但同一时刻只能处理一个控制器对象的逻辑），处理玩家交互逻辑
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"InputComponent.h"
#include"Collider.h"
#include"Object.h"

class Camera;

//玩家控制器类
class Controller:public Object
{
	InputComponent* inputcomponent;			///<输入交互组件 不希望派生类随便访问
protected:
	class Camera* camera ;				///相机组件
	float armLength = 20.f;				///<设置弹簧长度(控制屏幕缩放)

	/**
	 * 实现子类访问父类私有变量.
	 * @param [in] inputcomponent 在父类虚函数初始化中调用传入的实参就会是父类的变量
	 */
	virtual void SetupInputComponent(InputComponent* inputcomponent) ;			
public:
	Controller();

	virtual void Update(float deltaTime)override;

	virtual void BeginPlay() override;

	///更新点输入信息
	void PeekInfo() { inputcomponent->PeekInfo(); }

	///更新轴输入信息 就是一直按着的状态
	void PeekInfo_() { inputcomponent->PeekInfo_(); }

	///获取鼠标世界坐标系位置
	Vector2D GetMousePosition() const;

	///获取鼠标是否按下
	bool IsMouseClick()const;
	
	///该物体必须要有碰撞体 获取鼠标位置最上图层顺序的的物体
	Object* GetObjectUnderCursor();

	/**
	 * 获取世界坐标系下鼠标所处位置最顶层的物体信息（该物体必须拥有碰撞体）.
	 * 
	 * \return HitResult 世界坐标系下鼠标所处位置最顶层的物体信息
	 */
	HitResult GetHitResultUnderCursor();

	///激活输入输出
	void EnableInput(bool enable);

	virtual void DrawDebugPosition()const;

	///更新鼠标信息
	void MouseTick();

	/**
	* 设置相机可活动框架.
	 *
	* \param  [in] frame 相机可活动框(视野框）
	*/
	void SetRectFrame(const Rect& frame);

};