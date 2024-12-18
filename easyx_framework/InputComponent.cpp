#include "InputComponent.h"
#include"Windows.h"
#include <easyx.h>
#include "GameStatics.h"
void InputComponent::SetMapping(std::string mappingName, int value)
{
	mappings.insert({ mappingName,value });
}

void InputComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);
	//更新鼠标位置
	//POINT point;

	//Debug 2024/11.12 用win自带的获取是屏幕位置
	//GetCursorPos(&point);
	//mousePos = Vector2D((float)point.x, (float)point.y);

	//ExMessage msg;
	//if (peekmessage(&msg))
	//{
	//	mousePos = Vector2D(float(msg.x), float(msg.y));
	//}

	////更新按键控制逻辑
	//for (auto& mapping : mappings)
	//{
	//	KeyBindInfo& info = callbacks[mapping.first];
	//	if (GetAsyncKeyState(mapping.second) & 0x8000)
	//	{
	//		if (info.TYPE ==InputType::Pressed&&!info.isPressed) { info.fun(); }
	//		if (info.TYPE ==InputType::Holding) { info.fun(); }
	//		info.isPressed = true;
	//	}
	//	else if (info.isPressed)
	//	{
	//		if (info.TYPE == InputType::Released)info.fun();
	//		info.isPressed = false;
	//	}
	//}
}



//action
void InputComponent::PeekInfo()
{
	if (!bIsEnabled || !bActive)return;

	for (auto& mapping : mappings)
	{
		KeyBindInfo& info = actionCallbacks[mapping.first];
		if (info.TYPE == InputType::Holding)continue;
		if (GetAsyncKeyState((int)mapping.second) & 0x8000)
		{
			if (info.TYPE == InputType::Pressed && !info.isPressed)info.fun();
			if (info.TYPE == InputType::DoubleClick && info.lastTime > 0)
			{
				if (GameStatics::GetTimeSeconds() - info.lastTime < 0.5f) { info.fun(); info.lastTime = -1; }
				else info.lastTime = 0;
			}
			info.isPressed = true;
		}
		else if (info.isPressed)
		{
			if (info.TYPE == InputType::Released)info.fun();
			if (info.TYPE == InputType::DoubleClick && info.lastTime == 0)info.lastTime = GameStatics::GetTimeSeconds();
			if (info.TYPE == InputType::DoubleClick && info.lastTime == -1)info.lastTime = 0;
			info.isPressed = false;
		}
	}
}

//Holding
void InputComponent::PeekInfo_()
{
	if (!bIsEnabled || !bActive)return;

	for (auto& mapping : mappings)
	{
		KeyBindInfo& info = actionCallbacks[mapping.first];
		if (info.TYPE != InputType::Holding)continue;
		if (GetAsyncKeyState((int)mapping.second) & 0x8000)
		{
			info.fun();
		}
	}
}

//获取鼠标位置
Vector2D InputComponent::GetMousePosition() 
{
	return bActive ? mousePos : Vector2D{};
}

bool InputComponent::IsMouseButtonClick()
{
	return bActive ? (GetAsyncKeyState(VK_LBUTTON) & 0x8000) : false;
}

void InputComponent::EnableInput(bool enable)
{
	bActive = enable;
}

void InputComponent::MouseTick()
{
	if (peekmessage(&msg))
	{
		mousePos = Vector2D(float(msg.x), float(msg.y));
	}
}

Vector2D InputComponent::mousePos = {};

bool InputComponent::bActive = true;