/*****************************************************************//**
 * \file   SceneComponent.h
 * \brief  ���������
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"Component.h"


/**
 * �����������.
 */
class SceneComponent :public Component
{
	/*friend class SpriteRenderer;*/

	Transform transform;				///<��������
	std::set<SceneComponent*> children;		///<�󶨵ĺ�������
protected:
	SceneComponent* parent = nullptr;			///<��ָ��

	/**
	 * �������
	 * @remark ʹ�õݹ�������󶨲����� �������ĵݹ�ɾ���ڵ�.
	 * 
	 */
	void process_Destruct();

public:

	virtual void Update(float deltaTime)override {};

	///�����������
	void AttachTo(SceneComponent* par);

	/**
	 * ����������.
	 * @remark �������û�����
	 * \param [in] par��ָ��
	 */
	void DetachForm(SceneComponent* par);

	virtual void Destruct();

	virtual void BeginPlay()override {};

	virtual void EndPlay()override {};

	///��ȡ�������
	Vector2D GetLocalPosition() const { return transform.position; }			
	///��ȡ��ԽǶ�
	float GetLocalRotation() const { return transform.rotation; }			
	///��ȡ������Ŵ�С
	Vector2D GetLocalScale() const { return transform.scale; }				
	Vector2D GetWorldPosition()const;										//��ȡ��������
	float GetWorldRotation()const;											//��ȡ���ԽǶ�
	Vector2D GetWorldScale()const;											//��ȡ�������Ŵ�С
	///�����������
	void SetLocalPosition(const Vector2D& pos) { transform.position = pos; }			
	///������ԽǶ�
	void SetLocalRotation(float rot) { transform.rotation = rot; }					
	///����������Ŵ�С
	void SetLocalScale(const Vector2D& scale) { transform.scale = scale; }			
	///����һ������
	void AddPosition(const Vector2D& pos) { transform.position += pos; }		
	///����һ���Ƕ�
	void AddRotation(float rot) { transform.rotation += rot; }					
};