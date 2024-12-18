/*****************************************************************//**
 * \file   Animator.h
 * \brief  �����ӿڼ��������
 * @remark ���������붯��������ݵĹ����߼�
 * 
 * \author zhn
 *********************************************************************/

#pragma once
#include"easyx.h"
#include"Timer.h"
#include<unordered_map>
#include"Delegate.h"
#include"Component.h"
#include "CondiEdge.h"
//#include"FSM.h"

///����ί��(�޲ε���ί��)
///@brief ����:class AnimationDelegate:public UniCastDelegate<void>{};
DECLARE_NO_PARAM_UNICAST_DELEGATE(AnimationDelegate)


/**
 * �����ӿ�.
 * @brief֧��ʵ����̫�涯������Ҫ����
 * @brief��̫�涯�����൱��һ������������һ�����ܣ��ͷŵĽ���֮ǰ��֧��֮ǰ����������
 * @remark ������״̬ת���ж� ���Ӹ��ӵ�AI�����ο�״̬��
 * @see FSM
 */
class Animation
{	
	DECLARE_EDGE_CONDITION_BY_TYPE(AnimEdge,Animation)

	friend class Animator;
	friend class FsmAnimator;
	friend class AnimEdge;

	Animator* animController = nullptr;			///<���ĸ���������������
	Vector2D offset = { 0,0 };					///<����ƫ����
	int num = 0;								///<����֡��
	int index = 0;								///<��ǰ֡����
	IMAGE** images = nullptr;					///<����֡����
	bool bLooping = true;						///<�Ƿ��ѭ������

	AnimationDelegate OnMontageExit;		///<����������̫��Ķ����¼�
	bool bMontage = false;					///<�Ƿ�����̫�沥��

	Timer clock;												///<��ʱ�����չ̶�Ƶ��ʵ��Tick_()
	void UpdateIndex() { index = (index + 1) % num; };			///<��������

	std::unordered_map<int, AnimationDelegate> notification;		///<�Զ��嶯��ί�� ��ĳһ֡ʵ��ί��

	std::list<AnimEdge*>nexts;									///<������
public:
	///��ʱ��
	Animation() { clock.Bind(0.f, this, &Animation::Tick_, true); clock.Stop(); }
	
	/**
	 * ���ض�����Դ.
	 * 
	 * \param [in] name   ��������
	 * \param [in] delta ƫ����
	 */
	void Load(std::string name, Vector2D delta = { 0,0 });

	/**
	 * ����ʱ�Ӹ�������Ƶ��.
	 * 
	 * \param interval ����֡ʱ����
	 */
	void SetInterval(double interval) { clock.SetDelay(interval); }

	/**
	 * ���ö���֡�±�.
	 * 
	 * \param [in] i ���±�
	 */
	void SetIndex(int i) { index = i; }

	/**
	 *  ʱ��ִ�еĺ���.
	 * @remark ʵ�ֺ��Ĺ���
	 */
	void Tick_();

	/**
	 * �����Ƿ�ѭ������.
	 * 
	 * \param [in] loop �Ƿ�ѭ������
	 */
	void SetLooping(bool loop) { bLooping = loop; }

	/**
	 * ��Ӷ���ί��.
	 * 
	 * \param [in] index �±�
	 * \param [in] event �¼�
	 */
	void AddNotification(int index, AnimationDelegate& event) { notification.insert({index, event });}

	AnimationDelegate OnAnimEnter;			///<���붯���¼�

	AnimationDelegate OnAnimExit;			///<�뿪�����¼�

};

/**
* �������������
 * @brief һ�������кöද����һ��ִ��һ�� ���ǰѵ�ǰѡ��Ķ�����Դ������Ⱦ��������Ⱦ.
 * @remark ������úò������� ��������״̬��������AI���
 */
class Animator final:public Component
{
	DECLARE_EDGE_CONDITION_BY_TYPE(AnimEdge, Animation)

	friend class FSM;

	std::unordered_map<std::string, Animation&> animations;				///<����ӳ������ [�������֣�������Դ]

	std::unordered_map<std::string, int> parametersInteger;				///<ʵ��������ֵ�������� [�����������֣�ʵ����ֵ]
	std::unordered_map<std::string, float> parametersFloat;				///<ʵ�ʸ�������ֵ�������� [�����������֣�ʵ����ֵ]		
	std::unordered_map<std::string, bool> parametersBool;				///<ʵ�ʲ�������ֵ�������� [�����������֣�ʵ����ֵ]
	std::unordered_map<std::string, bool> parametersTrigger;			///<ʵ�ʴ�������ֵ�������� [�����������֣�ʵ����ֵ]

	Animation* aninode = nullptr;			///<��ǰ���Ŷ����ڵ�
	
	Animation* lastnode = nullptr;			///<��¼������̫��ǰ�Ķ���

	class SpriteRenderer* rendererAttached = nullptr;		///<���ŵ���Ⱦ��

	IMAGE* currentSprite = nullptr;						///<��ǰ���ŵ�ͼ��ָ��

	

	friend  class SpriteRenderer;
public:
	~Animator();
	virtual void Update(float deltaTime)override;

	virtual void  BeginPlay() ;

	void insert(std::string name, Animation& ani);

	Animation* GetNode()const { return aninode; };

	std::string GetName()const { for (auto& it : animations) { if (&(it.second) == (aninode))return it.first; }; }

	void SetNode(std::string name);

	void SetNode(Animation* node);

	/**
	 * ���ø�����Ⱦ��.
	 * 
	 * \param [in] renderer ��Ⱦ��
	 */
	void SetupAttachment(SpriteRenderer* renderer);

	/**
	 * ���ö���������״̬ ���õ�ǰִ�нڵ��ʱ����ʵ�ֿ���.
	 * 
	 * \param [in] called �Ƿ񲥷Ŷ���
	 */
	void SetCalled(bool called);

	/**
	 * ���Ŷ�����̫��Ƭ��.
	 * 
	 * \param [in] nodeName ��̫�涯��(��ͨ����)
	 */
	void PlayMontage(std::string nodeName);

	/**
	 * ��Ӳ�����ʵ��ֵ������.
	 * @remark ֻ��������������������������û��޸�ֵ
	 * \param [in] paramName ��������
	 * \param [in] type  ״̬��������
	 */
	void AddParamater(std::string paramName, ParamType type);

	/**
	 * ���û��޸�int����.
	 * 
	 * \param [in] paramName ��������
	 * \param [in] value ������ֵ
	 */
	void SetInteger(std::string paramName, int value);

	/**
	 * ����float����.
	 * 
	 * \param [in] paramName ��������
	 * \param [in] value ������ֵ
	 */
	void SetFloat(std::string paramName, float value);

	/**
	 * ����bool����.
	 * 
	 * \param [in] paramName ��������
	 * \param [in] value ������ֵ
	 */
	void SetBool(std::string paramName, bool value);

	/**
	 * ����������.
	 *
	 * \param [in] paramName ��������
	 */
	void SetTrigger(std::string paramName);

	/**
	 * �Ƿ���Ϲ�������.
	 * 
	 * \param edge ����������
	 * \return    true�������� false����������
	 */
	bool CheckConditions(AnimEdge* edge);

	/**
	 * �������.
	 * @remark ��SetCalled������һ���Ƕ����ӿڵ�ʱ��һ��������������
	 */
	virtual void Activate();

	/**
	 * ͣ�����.
	 * 
	 */
	virtual void Deactivate();
};

