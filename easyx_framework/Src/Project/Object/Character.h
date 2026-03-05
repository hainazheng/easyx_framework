#pragma once
/**
 * @file	Character.h
 * @brief   角色类定义，玩家控制器功能的延申，适合创建移动性较强的玩家操控的角色
 * @author	Zhn
 **/



#pragma once
#include "Controller.h"


 /* 角色状态 */
enum class CharacterMovementState : uint8
{
	Standing,		///< 站立
	Running,		///< 奔跑
	Flying			///< 飞行
};


/**
 * 角色基类.
 */
class Character : public Controller
{
	/*DEFINE_SUPER(Controller)*/

public:
	Character();

	virtual void BeginPlay()override;

	virtual void On_Update(float deltaTime)override;


	/**
	 * @brief 输入水平移动命令
	 * @param[in] inputValue			  移动值，向右为正
	 * @param[in] bControlScale           角色方向是否由输入值控制，默认为是
	 **/
	void AddInputX(float inputValue, bool bControlScale = true);

	//获取角色状态
	CharacterMovementState GetMovementState()const { return movementState; }

	///设置角色操作输入响应即时性
	void SetResponseFrame(int Frame) { Key_Response = Frame; }


protected:
	void OnLanded(Collider* hitComp, Collider* otherComp, Object* otherActor, Vector2D normalImpulse, const HitResult& hitResult);

private:
	class SpriteRenderer* render;
	class BoxCollider* box;
	class RigidBody* rigid;

	CharacterMovementState movementState;

	bool bAddMoving;//是否正在输入移动命令
	int MoveFlag;//输入标记
	int Key_Response;	//按键灵敏度
};