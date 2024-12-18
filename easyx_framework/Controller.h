/*****************************************************************//**
 * \file   Controller.h
 * \brief  ��ҿ������ඨ��
 * @remark����Ψһ����ͬһʱ��ֻ�ܴ���һ��������������߼�����������ҽ����߼�
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"InputComponent.h"
#include"Collider.h"
#include"Object.h"

class Camera;

//��ҿ�������
class Controller:public Object
{
	InputComponent* inputcomponent;			///<���뽻����� ��ϣ��������������
protected:
	class Camera* camera ;				///������
	float armLength = 20.f;				///<���õ��ɳ���(������Ļ����)

	/**
	 * ʵ��������ʸ���˽�б���.
	 * @param [in] inputcomponent �ڸ����麯����ʼ���е��ô����ʵ�ξͻ��Ǹ���ı���
	 */
	virtual void SetupInputComponent(InputComponent* inputcomponent) ;			
public:
	Controller();

	virtual void Update(float deltaTime)override;

	virtual void BeginPlay() override;

	///���µ�������Ϣ
	void PeekInfo() { inputcomponent->PeekInfo(); }

	///������������Ϣ ����һֱ���ŵ�״̬
	void PeekInfo_() { inputcomponent->PeekInfo_(); }

	///��ȡ�����������ϵλ��
	Vector2D GetMousePosition() const;

	///��ȡ����Ƿ���
	bool IsMouseClick()const;
	
	///���������Ҫ����ײ�� ��ȡ���λ������ͼ��˳��ĵ�����
	Object* GetObjectUnderCursor();

	/**
	 * ��ȡ��������ϵ���������λ������������Ϣ�����������ӵ����ײ�壩.
	 * 
	 * \return HitResult ��������ϵ���������λ������������Ϣ
	 */
	HitResult GetHitResultUnderCursor();

	///�����������
	void EnableInput(bool enable);

	virtual void DrawDebugPosition()const;

	///���������Ϣ
	void MouseTick();

	/**
	* ��������ɻ���.
	 *
	* \param  [in] frame ����ɻ��(��Ұ��
	*/
	void SetRectFrame(const Rect& frame);

};