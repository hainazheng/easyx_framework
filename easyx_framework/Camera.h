/*****************************************************************//**
 * \file   Camera.h
 * \brief  2D相机组件定义
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"SceneComponent.h"
#include"Object.h"

/**
 * 摄像机组件.
 */
class Camera:public SceneComponent
{
	friend class SpriteRenderer;
	friend class Particle;
	friend class Controller;
	friend class BoxCollider;
	friend class World;
	friend void Object::DrawDebugPosition()const;

	Transform camera_virtual;		///<虚拟相机场景属性

	Rect frame;						///<相机可活动框

	float distanceThreshold = 50.f;		///<线性阈值1~500
	short Smoothness = 50;				///<平滑度0~100


	float ShakeIndensity = 0.f;			///<震动强度 0~100
	bool shakeFlag = false;				///<震动标志位
	Vector2D shakeCenter = Vector2D();		///<震动中心
	Vector2D shakeSpeed = Vector2D(0.05f, 0.05f);		///<震动速度
	int shakeDecay = 5;						///<震动衰减系数1~100


	float springArmLength = 20.f;			///<弹簧长度1~10000
	float springArmLength_virtual = 100.f;		///<虚拟弹簧长度
	float smoothnessForSpringArm = 20.f;		///<弹簧平滑度0~100

	class Controller* controlAttached = nullptr;
	/**
	 * 线性插值.
	 * 
	 * \param [in] start 开始坐标 
	 * \param [in] end  结束坐标
	 * \param [in] t	系数 0.001~0.1(尽量小效果会好)
	 * \return 要插的值坐标
	 */
	Vector2D Lerp(Vector2D start, Vector2D end, float t);	

	/**
	 * 线性插值.
	 * 
	 * \param [in] start 开始值
	 * \param [in] end 结束值
	 * \param {in] t 系数 0.001~0.1(尽量小效果会好)
	 * \return 要插的值坐标
	 */
	float Lerp(float start, float end, float t);
	
	/**
	 * 平滑处理.
	 * 
	 * \param [in] x 要平滑处理的值
	 * \return 处理之后的值
	 */
	float SmoothStep(float x);
	
	/**
	 * 实现计算相机虚拟坐标.
	 * 
	 */
	void Calculate();
	
public:
	/**
	 * 构造函数
	 * 设置屏幕中心位置.
	 */
	Camera() { SetLocalPosition(Vector2D(WIN_WIDTH/2, WIN_HEIGHT/2)); };		//设置屏幕中心位置
	~Camera() {};

	virtual void BeginPlay()override;

	/**
	 * 获取相机虚拟位置.
	 * 
	 * \return 相机虚拟位置
	 */
	Vector2D GetCameraPosition()
	{
		return camera_virtual.position;
	}
	/**
	 * 获取虚拟弹簧长度.
	 * 
	 * \return 虚拟弹簧长度
	 */
	float GetSpringArmLength()
	{
		return springArmLength_virtual;
	}
	/**
	 * 设置世界主相机.
	 * 
	 */
	void SetMainCamera();

	
	/**
	 * 设置虚拟弹簧长度.
	 * 
	 * \param [in] length 弹簧长度
	 */
	void SetSpringArmLength(float length);

	/**
	 * 设置平滑度.
	 * 
	 * \param [in] smooth 平滑度
	 */
	void SetSmoothness(short smooth);

	/**
	 * 设置线性阈值.
	 * 
	 * \param  [in] threshold 线性阈值
	 */
	void SetDistanceThreshold(float threshold);

	/**
	 * 设置相机可活动框架.
	 * 
	 * \param  [in] frame 相机可活动框(视野框）
	 */
	void SetRectFrame(const Rect& frame);

	/**
	 * 震动相机
	 * 
	 * \param [in] indensity 震动强度
	 * \param [in] decay 震动衰减系数 默认为5
	 */
	void ShakeCamera(float indensity, int decay = 5);

	
};