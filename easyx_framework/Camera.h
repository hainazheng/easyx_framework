/*****************************************************************//**
 * \file   Camera.h
 * \brief  2D����������
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"SceneComponent.h"
#include"Object.h"

/**
 * ��������.
 */
class Camera:public SceneComponent
{
	friend class SpriteRenderer;
	friend class Particle;
	friend class Controller;
	friend class BoxCollider;
	friend class World;
	friend void Object::DrawDebugPosition()const;

	Transform camera_virtual;		///<���������������

	Rect frame;						///<����ɻ��

	float distanceThreshold = 50.f;		///<������ֵ1~500
	short Smoothness = 50;				///<ƽ����0~100


	float ShakeIndensity = 0.f;			///<��ǿ�� 0~100
	bool shakeFlag = false;				///<�𶯱�־λ
	Vector2D shakeCenter = Vector2D();		///<������
	Vector2D shakeSpeed = Vector2D(0.05f, 0.05f);		///<���ٶ�
	int shakeDecay = 5;						///<��˥��ϵ��1~100


	float springArmLength = 20.f;			///<���ɳ���1~10000
	float springArmLength_virtual = 100.f;		///<���ⵯ�ɳ���
	float smoothnessForSpringArm = 20.f;		///<����ƽ����0~100

	class Controller* controlAttached = nullptr;
	/**
	 * ���Բ�ֵ.
	 * 
	 * \param [in] start ��ʼ���� 
	 * \param [in] end  ��������
	 * \param [in] t	ϵ�� 0.001~0.1(����СЧ�����)
	 * \return Ҫ���ֵ����
	 */
	Vector2D Lerp(Vector2D start, Vector2D end, float t);	

	/**
	 * ���Բ�ֵ.
	 * 
	 * \param [in] start ��ʼֵ
	 * \param [in] end ����ֵ
	 * \param {in] t ϵ�� 0.001~0.1(����СЧ�����)
	 * \return Ҫ���ֵ����
	 */
	float Lerp(float start, float end, float t);
	
	/**
	 * ƽ������.
	 * 
	 * \param [in] x Ҫƽ�������ֵ
	 * \return ����֮���ֵ
	 */
	float SmoothStep(float x);
	
	/**
	 * ʵ�ּ��������������.
	 * 
	 */
	void Calculate();
	
public:
	/**
	 * ���캯��
	 * ������Ļ����λ��.
	 */
	Camera() { SetLocalPosition(Vector2D(WIN_WIDTH/2, WIN_HEIGHT/2)); };		//������Ļ����λ��
	~Camera() {};

	virtual void BeginPlay()override;

	/**
	 * ��ȡ�������λ��.
	 * 
	 * \return �������λ��
	 */
	Vector2D GetCameraPosition()
	{
		return camera_virtual.position;
	}
	/**
	 * ��ȡ���ⵯ�ɳ���.
	 * 
	 * \return ���ⵯ�ɳ���
	 */
	float GetSpringArmLength()
	{
		return springArmLength_virtual;
	}
	/**
	 * �������������.
	 * 
	 */
	void SetMainCamera();

	
	/**
	 * �������ⵯ�ɳ���.
	 * 
	 * \param [in] length ���ɳ���
	 */
	void SetSpringArmLength(float length);

	/**
	 * ����ƽ����.
	 * 
	 * \param [in] smooth ƽ����
	 */
	void SetSmoothness(short smooth);

	/**
	 * ����������ֵ.
	 * 
	 * \param  [in] threshold ������ֵ
	 */
	void SetDistanceThreshold(float threshold);

	/**
	 * ��������ɻ���.
	 * 
	 * \param  [in] frame ����ɻ��(��Ұ��
	 */
	void SetRectFrame(const Rect& frame);

	/**
	 * �����
	 * 
	 * \param [in] indensity ��ǿ��
	 * \param [in] decay ��˥��ϵ�� Ĭ��Ϊ5
	 */
	void ShakeCamera(float indensity, int decay = 5);

	
};