/*****************************************************************//**
 * \file   SpriteRenderer.h
 * \brief  2D��Ⱦ���
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"VisualInterface.h"
#include"Timer.h"
#include"SceneComponent.h"

/**
 * ��Ⱦ���(2D).
 */
class SpriteRenderer:public SceneComponent,public LayerInterface,public ImageInterface
{
	friend class Animator;
	friend class FsmAnimator;
	friend class Sprite;
	
	bool bmove = false;							///<�ж��Ƿ��Ѿ��ı�������λ��Ϊͼ������λ��

	 class Animator* animatorAttached = nullptr;		///<���ŵĶ���������

	 Timer WaveUpdate;					///<ˮ���Ƽ�ʱ��

	void DealImage();

	///����ͼ����Դ������
	void SaveImage();

	/**
	 * ����ͼ����Դ������.
	 * 
	 * \param img ��Ҫ�����ͼ����
	 */
	void SaveImage(IMAGE* img);

public:
	~SpriteRenderer() ;
	void BeginPlay();
	virtual void Render()override;
	virtual void Update(float deltaTime)override;
};


