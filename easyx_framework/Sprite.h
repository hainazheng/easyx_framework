/*****************************************************************//**
 * \file   Sprite.h
 * \brief  物体派生类(精灵类）
 * 
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"SpriteRenderer.h"
#include"Timer.h"
#include"CoreMinimal.h"

/**
 * 物体派生类(精灵类）
 * @brief 包含了渲染的物体.
 */
class Sprite :public Object
{
	SpriteRenderer* renderer;			///<渲染器组件 一个指针指向我们用物体创建出来的渲染组件

	float deltaTime=0.f;					///<帧的时间间隔

	bool IsBlinking = false;			///<是否正在闪烁状态		
	bool blinkFlag = false;				///<是否开始实现闪烁
	float transistionLevel = 0.f;			///<过渡层级
	float transistionSpeed = 0.f;			///<过渡速度
	FilterInfo blinkFillter;			///<闪烁滤镜信息
	Timer recoverTimerHandle;			///<恢复闪烁标识计时器
	void Recover();


public:
	Sprite();
	virtual void BeginPlay();
	virtual void Update(float deltaTime);
	void LoadSprite(std::string);			//加载图片资源
	void Blink(float duration = 0.2f, COLORREF col = BLACK, int level=60);
};