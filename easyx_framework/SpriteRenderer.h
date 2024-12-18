/*****************************************************************//**
 * \file   SpriteRenderer.h
 * \brief  2D渲染组件
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"VisualInterface.h"
#include"Timer.h"
#include"SceneComponent.h"

/**
 * 渲染组件(2D).
 */
class SpriteRenderer:public SceneComponent,public LayerInterface,public ImageInterface
{
	friend class Animator;
	friend class FsmAnimator;
	friend class Sprite;
	
	bool bmove = false;							///<判断是否已经改变了坐标位置为图像中心位置

	 class Animator* animatorAttached = nullptr;		///<附着的动画播放器

	 Timer WaveUpdate;					///<水波纹计时器

	void DealImage();

	///保存图像资源到本地
	void SaveImage();

	/**
	 * 保存图像资源到本地.
	 * 
	 * \param img 想要保存的图像句柄
	 */
	void SaveImage(IMAGE* img);

public:
	~SpriteRenderer() ;
	void BeginPlay();
	virtual void Render()override;
	virtual void Update(float deltaTime)override;
};


