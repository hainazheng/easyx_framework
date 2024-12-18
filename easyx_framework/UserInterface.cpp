#include"UserInterface.h"
#include"World.h"

///���캯��
/// @remark Ĭ���Ǹ�����ͬ����С ��������
UserInterface::UserInterface()
{
	root->SetSize({ float(getwidth()), float(getheight()) });
	if (root->GetPosPattern() == LayoutPositionPattern::Center)root->SetRelativePosition({ float(getwidth() * 0.5),float(getheight() * 0.5) });
}

///ֻҪ�ǿ��ӵģ�ִ��ÿһ��Widget��Update
void UserInterface::Update(float deltaTime)
{
	for (auto& widget : widgets)
	{
		if(widget->GetUiPattern()!=UiPattern::None)widget->Update();
		
	}
}

///���Ʋ����ı߿�
void UserInterface::DrawDebugRect()
{
	for (auto& widget : widgets) { widget->DrawDebugRect(); }
}

///����Ϊ���Ӳ��ҿɽ���
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

///ɾ��ui ���뵽ɾ������ת������Ȩ��ͳһ����ɾ��
void UserInterface::RemoveFromViewport()
{
	mainWorld.GameUIs_to_delete.insert(this);
}

