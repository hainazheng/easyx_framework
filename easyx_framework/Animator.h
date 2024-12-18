/*****************************************************************//**
 * \file   Animator.h
 * \brief  动画接口及动画组件
 * @remark 包含所有与动画相关内容的管理逻辑
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

///动画委托(无参单播委托)
///@brief 类似:class AnimationDelegate:public UniCastDelegate<void>{};
DECLARE_NO_PARAM_UNICAST_DELEGATE(AnimationDelegate)


/**
 * 动画接口.
 * @brief支持实现蒙太奇动画，需要设置
 * @brief蒙太奇动画即相当于一个动作，比如一个技能，释放的结束之前不支持之前的其他操作
 * @remark 本身有状态转换判定 更加复杂的AI设计请参考状态机
 * @see FSM
 */
class Animation
{	
	DECLARE_EDGE_CONDITION_BY_TYPE(AnimEdge,Animation)

	friend class Animator;
	friend class FsmAnimator;
	friend class AnimEdge;

	Animator* animController = nullptr;			///<由哪个动画播放器播放
	Vector2D offset = { 0,0 };					///<像素偏移量
	int num = 0;								///<动画帧数
	int index = 0;								///<当前帧索引
	IMAGE** images = nullptr;					///<动画帧数组
	bool bLooping = true;						///<是否可循环播放

	AnimationDelegate OnMontageExit;		///<结束播放蒙太奇的动画事件
	bool bMontage = false;					///<是否处于蒙太奇播放

	Timer clock;												///<计时器按照固定频率实现Tick_()
	void UpdateIndex() { index = (index + 1) % num; };			///<更新索引

	std::unordered_map<int, AnimationDelegate> notification;		///<自定义动画委托 在某一帧实现委托

	std::list<AnimEdge*>nexts;									///<动画桥
public:
	///绑定时钟
	Animation() { clock.Bind(0.f, this, &Animation::Tick_, true); clock.Stop(); }
	
	/**
	 * 加载动画资源.
	 * 
	 * \param [in] name   动画名字
	 * \param [in] delta 偏移量
	 */
	void Load(std::string name, Vector2D delta = { 0,0 });

	/**
	 * 设置时钟更新索引频率.
	 * 
	 * \param interval 动画帧时间间隔
	 */
	void SetInterval(double interval) { clock.SetDelay(interval); }

	/**
	 * 设置动画帧下标.
	 * 
	 * \param [in] i 新下标
	 */
	void SetIndex(int i) { index = i; }

	/**
	 *  时钟执行的函数.
	 * @remark 实现核心功能
	 */
	void Tick_();

	/**
	 * 设置是否循环播放.
	 * 
	 * \param [in] loop 是否循环播放
	 */
	void SetLooping(bool loop) { bLooping = loop; }

	/**
	 * 添加动画委托.
	 * 
	 * \param [in] index 下标
	 * \param [in] event 事件
	 */
	void AddNotification(int index, AnimationDelegate& event) { notification.insert({index, event });}

	AnimationDelegate OnAnimEnter;			///<进入动画事件

	AnimationDelegate OnAnimExit;			///<离开动画事件

};

/**
* 动画播放器组件
 * @brief 一个物体有好多动画，一次执行一个 就是把当前选择的动画资源交给渲染器进行渲染.
 * @remark 如果设置好参数更新 可以类似状态机来进行AI设计
 */
class Animator final:public Component
{
	DECLARE_EDGE_CONDITION_BY_TYPE(AnimEdge, Animation)

	friend class FSM;

	std::unordered_map<std::string, Animation&> animations;				///<动画映射容器 [动画名字，动画资源]

	std::unordered_map<std::string, int> parametersInteger;				///<实际整形数值条件容器 [条件参数名字，实际数值]
	std::unordered_map<std::string, float> parametersFloat;				///<实际浮点型数值条件容器 [条件参数名字，实际数值]		
	std::unordered_map<std::string, bool> parametersBool;				///<实际布尔型数值条件容器 [条件参数名字，实际数值]
	std::unordered_map<std::string, bool> parametersTrigger;			///<实际触发型数值条件容器 [条件参数名字，实际数值]

	Animation* aninode = nullptr;			///<当前播放动画节点
	
	Animation* lastnode = nullptr;			///<记录播放蒙太奇前的动画

	class SpriteRenderer* rendererAttached = nullptr;		///<附着的渲染器

	IMAGE* currentSprite = nullptr;						///<当前播放的图像指针

	

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
	 * 设置附着渲染器.
	 * 
	 * \param [in] renderer 渲染器
	 */
	void SetupAttachment(SpriteRenderer* renderer);

	/**
	 * 设置动画播放器状态 利用当前执行节点计时器来实现控制.
	 * 
	 * \param [in] called 是否播放动画
	 */
	void SetCalled(bool called);

	/**
	 * 播放动画蒙太奇片段.
	 * 
	 * \param [in] nodeName 蒙太奇动画(普通动画)
	 */
	void PlayMontage(std::string nodeName);

	/**
	 * 添加参数到实际值容器中.
	 * @remark 只有容器中有这个参数名才能设置或修改值
	 * \param [in] paramName 参数名字
	 * \param [in] type  状态参数类型
	 */
	void AddParamater(std::string paramName, ParamType type);

	/**
	 * 设置或修改int参数.
	 * 
	 * \param [in] paramName 参数名字
	 * \param [in] value 整数数值
	 */
	void SetInteger(std::string paramName, int value);

	/**
	 * 设置float参数.
	 * 
	 * \param [in] paramName 参数名字
	 * \param [in] value 浮点数值
	 */
	void SetFloat(std::string paramName, float value);

	/**
	 * 设置bool参数.
	 * 
	 * \param [in] paramName 参数名字
	 * \param [in] value 布尔数值
	 */
	void SetBool(std::string paramName, bool value);

	/**
	 * 启动触发器.
	 *
	 * \param [in] paramName 参数名字
	 */
	void SetTrigger(std::string paramName);

	/**
	 * 是否符合过渡条件.
	 * 
	 * \param edge 动画过渡类
	 * \return    true满足条件 false不满足条件
	 */
	bool CheckConditions(AnimEdge* edge);

	/**
	 * 激活组件.
	 * @remark 跟SetCalled差别就是一个是动画接口的时钟一个是组件整体控制
	 */
	virtual void Activate();

	/**
	 * 停用组件.
	 * 
	 */
	virtual void Deactivate();
};

