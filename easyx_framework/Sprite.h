/*****************************************************************//**
 * \file   Sprite.h
 * \brief  ����������(�����ࣩ
 * 
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"SpriteRenderer.h"
#include"Timer.h"
#include"CoreMinimal.h"

/**
 * ����������(�����ࣩ
 * @brief ��������Ⱦ������.
 */
class Sprite :public Object
{
	SpriteRenderer* renderer;			///<��Ⱦ����� һ��ָ��ָ�����������崴����������Ⱦ���

	float deltaTime=0.f;					///<֡��ʱ����

	bool IsBlinking = false;			///<�Ƿ�������˸״̬		
	bool blinkFlag = false;				///<�Ƿ�ʼʵ����˸
	float transistionLevel = 0.f;			///<���ɲ㼶
	float transistionSpeed = 0.f;			///<�����ٶ�
	FilterInfo blinkFillter;			///<��˸�˾���Ϣ
	Timer recoverTimerHandle;			///<�ָ���˸��ʶ��ʱ��
	void Recover();


public:
	Sprite();
	virtual void BeginPlay();
	virtual void Update(float deltaTime);
	void LoadSprite(std::string);			//����ͼƬ��Դ
	void Blink(float duration = 0.2f, COLORREF col = BLACK, int level=60);
};