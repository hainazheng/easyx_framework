#include"UserInterface.h"
#include"World.h"

///构造函数
/// @remark 默认是跟窗口同样大小 中心坐标
UserInterface::UserInterface()
{
	root->SetSize({ float(getwidth()), float(getheight()) });
	if (root->GetPosPattern() == LayoutPositionPattern::Center)root->SetRelativePosition({ float(getwidth() * 0.5),float(getheight() * 0.5) });
}

///只要是可视的，执行每一个Widget的Update
void UserInterface::Update(float deltaTime)
{
	for (auto& widget : widgets)
	{
		if(widget->GetUiPattern()!=UiPattern::None)widget->Update();
		
	}
}

///绘制部件的边框
void UserInterface::DrawDebugRect()
{
	for (auto& widget : widgets) { widget->DrawDebugRect(); }
}

///设置为可视并且可交互
void UserInterface::AddToViewport()
{
	for (auto& widget : widgets)
	{
		widget->SetUiPattern(UiPattern::VisibleAndInteractive);
	}
}

void UserInterface::HideFromViewport()
{
	for (auto& widget : widgets)
	{
		widget->SetUiPattern(UiPattern::None);
	}
}

///删除ui 加入到删除容器转交控制权，统一进行删除
void UserInterface::RemoveFromViewport()
{
	mainWorld.GameUIs_to_delete.insert(this);
}

