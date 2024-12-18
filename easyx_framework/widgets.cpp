#include "widgets.h"
#include"Math.h"
#include"InputComponent.h"
#include"UserInterface.h"
#include"GameStatics.h"
#include"Controller.h"
#include"ResourceManager.h"
std::unordered_map<std::string, COLORREF>Characters::TextColorMap =
{
	{"$0",WHITE},{"$1",RGB(245,245,245)},{"$2",LIGHTGRAY},{"$3",DARKGRAY}, {"$4",BLACK},
	{"$5",GREEN},{"$6",LIGHTGREEN}, {"$7",YELLOW},
	{"$8",RGB(255, 165, 0)},{"$9",LIGHTRED}, {"$a",RED},
	{"$b",LIGHTMAGENTA},{"$c",MAGENTA}, {"$d",RGB(148, 0, 211)},
	{"$e",BLUE},{"$f",LIGHTBLUE}, {"$g",LIGHTCYAN},
	{"$h",CYAN},{"$i",BROWN}

};

void Characters::SetCharacters(std::string text, int size, LPCTSTR type)
{
	//��ʼ��
	row = 1; colume = 0;
	int temp = 0;
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] == '\n')
		{
			row++; colume = temp > colume ? temp : colume; temp = 0;
		}
		else if (text[i] == '$' && i + 1 < text.length())
		{
			std::string buf = "$" + text[i + 1];
			if (TextColorMap.find(buf) != TextColorMap.end())
			{
				++i;
			}
			else temp++;
		}
		else temp++;
		colume = temp > colume ? temp : colume;
		this->texts = text;
		this->size = size;
		this->type = type;
	}

}



void Characters::PrintCharacters(Vector2D pos, CharactersPattern pattern)
{
	this->pos = pos;
	settextstyle(6 * size, 3 * size, type);
	COLORREF color = BLACK;
	settextcolor(color);
	int r = 0;
	//ÿһ�е�����
	std::string temp;
	int pat = 1;
	switch (pattern)
	{
	case CharactersPattern::Left:pat = 0; break;
	case CharactersPattern::Middle:pat = 1; break;
	case CharactersPattern::Right:pat = 2; break;
	default:pat = 1; break;
	}
	for (int i = 0; i < texts.length(); i++)
	{
		if (texts[i] == '\n')
		{

			outtextxy((int)pos.x + (colume * 3 * size - int(temp.size()) * 3 * size) * pat / 2, pos.y + 6 * r * size, LPCTSTR(temp.c_str()));
			temp.clear(); ++r;
		}
		else if (texts[i] == '$' && i + 1 < texts.length())
		{
			std::string buf = texts.substr(i, 2);
			if (TextColorMap.find(buf) != TextColorMap.end())
			{
				color = TextColorMap[buf];
				settextcolor(color);
				++i;
			}
			else temp.push_back(texts[i]);
		}
		else temp.push_back(texts[i]);
	}
	if (!temp.empty())
	{
		outtextxy((int)pos.x + (colume * 3 * size - int(temp.size()) * 3 * size) * pat / 2, pos.y + 6 * r * size, LPCTSTR(temp.c_str()));
		temp.clear(); r++;
	}
	settextcolor(BLACK);
}

void Characters::debugline()
{
	Vector2D pos_m = Vector2D((int)pos.x + colume * 3 * size, pos.y + row * 6 * size);
	rectangle(pos.x, pos.y, pos_m.x, pos_m.y); 
}

Vector2D Widget::GetLayoutOffset() const
{
	switch (layoutpattern)
	{
	case LayoutPattern::LeftTop:if (parent->pospattern == LayoutPositionPattern::LeftTop)return Vector2D(0, 0); else { return Vector2D(-parent->GetSize().x / 2, -parent->GetSize().y / 2); }
	case LayoutPattern::MiddleTop:if (parent->pospattern == LayoutPositionPattern::LeftTop)return Vector2D(parent->GetSize().x/2, 0); else{ return Vector2D(0, -parent->GetSize().y / 2); }
	case LayoutPattern::RightTop:if (parent->pospattern == LayoutPositionPattern::LeftTop)return Vector2D(parent->GetSize().x, 0); else { return Vector2D(parent->GetSize().x / 2, -parent->GetSize().y / 2); }
	case LayoutPattern::LeftMiddle:if (parent->pospattern == LayoutPositionPattern::LeftTop)return Vector2D(0, parent->GetSize().y/2); else { return Vector2D(-parent->GetSize().x / 2, 0); }
	case LayoutPattern::Center:if (parent->pospattern == LayoutPositionPattern::LeftTop)return parent->GetSize() / 2; else { return Vector2D(0, 0); }
	case LayoutPattern::RightMiddle:if (parent->pospattern == LayoutPositionPattern::LeftTop)return Vector2D(parent->GetSize().x, parent->GetSize().y/2); else { return Vector2D(parent->GetSize().x / 2, 0); }
	case LayoutPattern::LeftBottom:if (parent->pospattern == LayoutPositionPattern::LeftTop)return Vector2D(0, parent->GetSize().y); else { return Vector2D(-parent->GetSize().x / 2, parent->GetSize().y / 2); }
	case LayoutPattern::MiddleBottom:if (parent->pospattern == LayoutPositionPattern::LeftTop)return Vector2D(parent->GetSize().x/2, parent->GetSize().y); else { return Vector2D(0, parent->GetSize().y / 2); }
	case LayoutPattern::RightBottom:if (parent->pospattern == LayoutPositionPattern::LeftTop)return Vector2D(parent->GetSize().x, parent->GetSize().y); else { return Vector2D(parent->GetSize().x / 2, parent->GetSize().y / 2); }
	default:return Vector2D(0, 0);
	}
	return Vector2D(0,0);
}

bool Widget::isUnderCursor() const
{
	if (uipattern != UiPattern::VisibleAndInteractive)return false;
	Vector2D loc = InputComponent::GetMousePosition();
	/*std::cout <<"pos" << loc << std::endl;*/
	int i = Math::Clamp(int(loc.x) / 100, 0, 7);
	int j = Math::Clamp(int(loc.y) / 100, 0, 5);

	if (mainWorld.UiDetectZones[i][j].empty())return false;
	
		for (auto it = mainWorld.UiDetectZones[i][j].rbegin(); it != mainWorld.UiDetectZones[i][j].rend(); it++)
		{
			if (Widget* widget = Cast<Widget>(*it))
			{
				Vector2D pos = widget->GetScreenPosition();
				
				if (pospattern == LayoutPositionPattern::Center)
				{
					if (widget == this && loc.x<pos.x + GetSize().x / 2 && loc.x > pos.x - GetSize().x / 2 && loc.y < pos.y + GetSize().y / 2 && loc.y > pos.y - GetSize().y / 2)
					{
						return true; std::cout << 1 << std::endl;
					}
				}
				else if (pospattern == LayoutPositionPattern::LeftTop)
				{
					if (widget == this && loc.x<pos.x + GetSize().x  && loc.x > pos.x  && loc.y < pos.y + GetSize().y  && loc.y > pos.y )
						return true;
				}
			}
		}
	
	return false;
}

///��������
///��mainworld����������ɾȥ��
Widget::~Widget()
{
	for (int i = point_lt.x; i <= point_rb.x; i++)for (int j = point_lt.y; j <= point_rb.y; j++)mainWorld.UiDetectZones[i][j].erase(this);
}

///�������������н���һЩ��������
/// ÿһ֡gengxin
void Widget::Update()
{
	//ʵ�ֽ���
	if (uipattern != UiPattern::VisibleAndInteractive)return;
	Vector2D pos(0,0); 
	if (pospattern == LayoutPositionPattern::Center)
		pos = GetScreenPosition() - GetSize() / 2;	//��������ϵ ��ȡ���Ͻǵ�����
	else if (pospattern == LayoutPositionPattern::LeftTop)
		pos = GetScreenPosition();		//���Ͻ�����ϵ
	Pair newpoint(Math::Clamp(int(pos.x/100),0,7),Math::Clamp(int(pos.y/100),0,5));
	pos += GetSize();
	Pair newpoint_1(Math::Clamp(int(pos.x / 100), 0, 7), Math::Clamp(int(pos.y/100),0,5));
	if (newpoint == point_lt && newpoint_1 == point_rb)return;

	if (point_lt != Pair(-1, -1))for (int i = point_lt.x; i <= point_rb.x; i++)for (int j = point_lt.y; j <= point_rb.y; j++)mainWorld.UiDetectZones[i][j].erase(this);
	point_lt = newpoint; point_rb = newpoint_1;
	for (int i = point_lt.x; i <= point_rb.x; i++)for (int j = point_lt.y; j <= point_rb.y; j++)mainWorld.UiDetectZones[i][j].insert(this);

}

///��ʾ��ʾ��
void Widget::ShowInfoBox()
{

	if (bInfoBox && uipattern == UiPattern::VisibleAndInteractive && isUnderCursor())
	{
		setlinecolor(BLACK);
		setlinecolor(PS_JOIN_BEVEL | PS_SOLID);
		setfillcolor(RGB(255, 247, 213));

		Vector2D pos = InputComponent::GetMousePosition() + Vector2D(-15, 15);
		fillrectangle(int(pos.x), int(pos.y), int(pos.x + InfoText.GetWidth()), int(pos.y + InfoText.GetHeight()));
		InfoText.PrintCharacters(pos);
	}
}

///���Ʋ����ı߿�
void Widget::DrawDebugRect()
{
	setlinecolor(BLUE);
	Vector2D pos = { 0,0 };
	if (pospattern == LayoutPositionPattern::Center)
		pos = GetScreenPosition() - GetSize() / 2;	//��������ϵ ��ȡ���Ͻǵ�����
	else if (pospattern == LayoutPositionPattern::LeftTop)
		pos = GetScreenPosition();		//���Ͻ�����ϵ
	int left = int(pos.x), top = int(pos.y);
	pos += GetSize();
	int right = int(pos.x), bottom = int(pos.y);
	rectangle(left, top, right, bottom);
}

///���ò���������
void Widget::SetUiPattern(UiPattern pattern)
{
	uipattern = pattern;
	for (auto& child : children)
	{
		child->SetUiPattern(pattern);
	}
	if (uipattern != UiPattern::VisibleAndInteractive)
	{
		if (point_lt != Pair(-1, -1))for (int i = point_lt.x; i <= point_rb.x; ++i)for (int j = point_lt.y; j <= point_rb.y; ++j)mainWorld.UiDetectZones[i][j].erase(this);
		point_lt = { -1, -1 }, point_rb = { -1, -1 };
	}
}

///��ȡʵ�ʴ�С
Vector2D Widget::GetSize() const
{
	return size*GetScreenScale();			//��С*����
}

void Widget::AttachTo(Widget* par)
{
	if (par)
	{
		par->children.insert(this);
		parent = par;
	}
}


///�������С�����İ�
/// @remark !!!�û�����
void Widget::DetachFrom(Widget* par)
{
	if (par)
	{
		par->children.erase(this);
		parent = nullptr;
	}
}

Vector2D Widget::GetScreenPosition() const
{
	if (parent)return GetRelativePosition() +GetLayoutOffset() + parent->GetScreenPosition();
	else return GetRelativePosition();
}

///��ȡ�����Ļ����ϵ�ĽǶ�
float Widget::GetScreenRotation() const
{
	if (parent)return GetRelativeRotation() + parent->GetScreenRotation();
	else return GetRelativeRotation();
}

///��ȡ���Է�������
Vector2D Widget::GetScreenScale() const
{
	if (parent)return GetRelativeScale() * parent->GetScreenScale();
	else return GetRelativeScale();
}

Panel::Panel()
{
}

///��������
/// @brief �����а󶨵ĳ�Ա������
Panel::~Panel()
{
	if (members_ui.empty())return;
	for (auto& ui : members_ui)
	{
		ui->RemoveFromViewport();
	}
}

///���°󶨳�Աλ��
void Panel::Update()
{
	Widget::Update();
	if (members.empty())return;
	int32 index = 0;
	for (auto& member : members)
	{
		AdjustMemberPosition(member, index++);
	}
}

void Panel::SetUnitSize(Vector2D size)
{
	UnitSize = size;
	Panel::Update();
}

void Panel::AddMember(Widget* member, int32 index)
{
	member->AttachTo(this);
	
	if (index)members.emplace(members.begin() + index, member);
	else members.push_back(member);
	member->attachedPanel=this;
	AdjustMemberSizeToUnit(member);
	/*if (index)AdjustMemberPosition(member, index);*/
	AdjustMemberPosition(member, index >= 0 ? index : members.size() - 1);
}

void Panel::Remove(Widget* member)
{
	member->DetachFrom(this);
	members.erase(std::remove(members.begin(),members.end(),member),members.end());			//vectorɾ��remove���ص���û�б�ɾ�������һ��Ԫ�ص�λ��
	member->attachedPanel = nullptr;
}

void Panel::AddMember(UserInterface* member, int32 index)
{
	member->root->AttachTo(this);
	index = Math::Clamp(index, -1, int32(members.size()));
	if (index>=0)members.emplace(members.begin() + index, member->root);
	else members.push_back(member->root);
	member->root->attachedPanel=this;
	members_ui.push_back(member);
	AdjustMemberSizeToUnit(member->root);
	/*if (index)AdjustMemberPosition(member->root, index);*/
	AdjustMemberPosition(member->root, index >= 0 ? index : members.size() - 1);/*std::cout << "member" << GetRelativePosition() << std::endl;*/
}

void Panel::Remove(UserInterface* member)
{
	member->root->DetachFrom(this);
	members.erase(std::remove(members.begin(), members.end(), member->root), members.end());			//vectorɾ��remove���ص���û�б�ɾ�������һ��Ԫ�ص�λ��
	members_ui.erase(std::remove(members_ui.begin(), members_ui.end(), member), members_ui.end());
	member->root->attachedPanel = nullptr;
}

///���ı�ԭ����С���ı���������ܹ��ָ�ԭ����С ֻҪ���ڻ�(1,1)����
void Panel::AdjustMemberSizeToUnit(Widget* member)
{
	member->SetRelativeScale(UnitSize/member->GetSize());
}

//Vector2D Panel::GetScreenPosition() const
//{
//	/*if (parent&&parent->GetPosPattern()==LayoutPositionPattern::Center)return GetRelativePosition() + GetLayoutOffset() + parent->GetScreenPosition()-parent->GetSize()/2;*/
//	//if (parent && parent->GetPosPattern() == LayoutPositionPattern::Center&& GetPosPattern() == LayoutPositionPattern::LeftTop)return GetRelativePosition() + GetLayoutOffset() + parent->GetScreenPosition()/* + GetSize() / 2*/;
//	//else 
//	 if (parent)return GetRelativePosition() + GetLayoutOffset() + parent->GetScreenPosition();
//	else return GetRelativePosition();
//}

///ˮƽ���и�������
void HorizontalPanel::AdjustMemberPosition(Widget* member, int32 index)
{
	if (index<0)return;
	Vector2D pos = Vector2D(index * (spacing + UnitSize.x), 0);
	if(member->GetPosPattern() == LayoutPositionPattern::Center)
	pos = pos+ UnitSize * 0.5f;			//��ÿһ��ͼת������������λ��

	member->SetRelativePosition(pos);
	
}

///ʵ�ʴ�С=(��Ա����*UnitSizex��С,UnitSizey��С)+((����-1)*��϶,UnitSizey��С)
Vector2D HorizontalPanel::GetSize() const
{
	return (members.empty()) ? Vector2D(0, 0) : Vector2D(members.size()* (spacing + UnitSize.x) - spacing, UnitSize.y);
}

//Vector2D HorizontalPanel::GetScreenPosition() const
//{
//	if (parent && parent->GetPosPattern() == LayoutPositionPattern::Center)return GetRelativePosition() + GetLayoutOffset() + parent->GetScreenPosition() - GetSize() / 2;
//	else if (parent)return GetRelativePosition() + GetLayoutOffset() + parent->GetScreenPosition();
//	else return GetRelativePosition();
//}

///��ֱ���и�������
void VerticalPanel::AdjustMemberPosition(Widget* member, int32 index)
{
	if (index<0)return;
	Vector2D pos = Vector2D(0, index * (spacing + UnitSize.y));
	if (member->GetPosPattern() == LayoutPositionPattern::Center)
	pos = pos + UnitSize * 0.5f;
	member->SetRelativePosition(pos);
}

///ʵ�ʴ�С=(UnitSizex��С,��Ա����*UnitSizex��С)+(UnitSizex��С,(����-1)*��϶)
Vector2D VerticalPanel::GetSize() const
{
	return (members.empty()) ? Vector2D(0, 0) : Vector2D(UnitSize.x, members.size() * (spacing + UnitSize.y) - spacing);
}

///�������и�������
void GridPanel::AdjustMemberPosition(Widget* member, int32 index)
{
	if (!index)return;
	//����Ӧ
	priority ? column : row = priority ? (index / row + 1) : (index % column + 1);
	Vector2D pos = Vector2D(int32( priority? index /row: index % column) * (spacingx + UnitSize.x), int32(priority ? index % row : index / column) * (spacingy + UnitSize.y));
	if (member->GetPosPattern() == LayoutPositionPattern::Center)
	pos = pos + UnitSize * 0.5f;
	member->SetRelativePosition(pos);
}

Vector2D GridPanel::GetSize() const
{
	return (members.empty()) ? Vector2D(0, 0) : Vector2D(column * (spacingx + UnitSize.x) - spacingx, row * (spacingy + UnitSize.y) - spacingy);
}

///�����ı�������ı���ʽ
void Text::Update()
{
	Widget::Update();
	if(BindedText)texts.SetCharacters(*BindedText);
	if(GetPosPattern()==LayoutPositionPattern::Center)
	size = Vector2D(float(texts.GetWidth()), float(texts.GetHeight()));

}

///��Ⱦ�ı�
void Text::Render()
{
	//2024.11.18 DEBUG �ı�û�а����������������Ⱦ
	if (GetPosPattern() == LayoutPositionPattern::Center)
	texts.PrintCharacters(GetScreenPosition() - size * 0.5f, textpattern);

#ifdef _DEBUG
	texts.debugline();
#endif
}

void Image::Update()
{
	Widget::Update();
	if (!sprite)return;

	if (copy)spriteinfo.size = Pair(copy->getwidth(), copy->getheight());
	else spriteinfo.size = Pair(sprite->getwidth(), sprite->getheight());

	//��ת��ʱ���˾�����
	if (GetScreenRotation() != angle)
	{
		angle = GetScreenRotation();
		RotateImage(angle * PI / 180.f);
		if ( bFilterEnable && FilterLayer.size() > 0)FilterImage();
	}

}

void Image::Render()
{
	if (!sprite)return;
	//��ȾĬ��posΪ��������

	/*std::cout << "joinui" << std::endl;*/
	Vector2D pos = { 0,0 };
	if (pospattern == LayoutPositionPattern::Center)
		pos = GetScreenPosition();	//��������ϵ ��ȡ���Ͻǵ�����
	else if (pospattern == LayoutPositionPattern::LeftTop)
		pos = GetScreenPosition() + GetSize() / 2;;		//���Ͻ�����ϵ

		//�ж��Ƿ��ж�ͼ�ν�����ת����
		IMAGE* img = copy ? copy : sprite;

		HDC dstDC = GetImageHDC();				//����ƽ���ͼƬ���
		HDC srcDC = GetImageHDC(img);			//Ҫ���ݵ�ͼƬ���
		int w = spriteinfo.EndLoc.x -spriteinfo.StartLoc.x;			//IMAGE�����Դ���ȡ����Ŀ��
		int h = spriteinfo.EndLoc.y - spriteinfo.StartLoc.y;		//IMAGE�����Դ���ȡ����ĸ߶�

		int dw = int(GetSize().x * w / img->getwidth());
		int dh = int(GetSize().y * h / img->getheight());
		
		/*std::cout << (int)pos.x - dw / 2 <<" "<< (int)pos.y - dh / 2 << std::endl;*/

		BLENDFUNCTION bf = { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA };				//����͸���ȣ���Աȫ��BYTE����
		//�Ƿ�������˾�
		if (bFilterEnable && filter && FilterLayer.size() > 0)
		{
			HDC srcDC_filter = GetImageHDC(filter);
			AlphaBlend(dstDC, (int)pos.x - GetSize().x / 2, (int)pos.y - GetSize().y / 2, dw, dh, srcDC_filter, (int)spriteinfo.StartLoc.x, (int)spriteinfo.StartLoc.y, w, h, bf);			//�൱��putimage ճ������Ӧ�Ĺ���ƽ��

		}
		else
			AlphaBlend(dstDC, (int)pos.x - GetSize().x / 2, (int)pos.y - GetSize().y / 2, dw, dh, srcDC, (int)spriteinfo.StartLoc.x, (int)spriteinfo.StartLoc.y, w, h, bf);			//�൱��putimage ճ������Ӧ�Ĺ���ƽ��
	
}

//����Ƿ��ڲ�����
bool Image::IsMouseOn()
{

	return isUnderCursor();
}

///ʵ�ֹ���
void Button::Update()
{
	Image::Update();
	//�ж�����¼�
	if (!ishover && IsMouseOn())
	{
		OnMouseHoverBegin.BroadCast();
		ishover = true;
	}
	else if (ishover && !IsMouseOn())
	{
		OnMouseHoverEnd.BroadCast();
		ishover = false;
	}

	if (!isPressed && IsMousePressed())
	{
		OnMousePressedBegin.BroadCast();
		isPressed = true;
	}
	else if(isPressed && !IsMousePressed())
	{
		OnMousePressedEnd.BroadCast();
		isPressed = false;
	}

	//�������ͼƬ����û��
	if (!hover && !pressed)
	{
		sprite = normal; return;
	}

	if (!ishover) { sprite = normal; spriteinfo.EndLoc.x = normal->getwidth(); spriteinfo.EndLoc.y = normal->getheight(); }
	else if (!isPressed) { sprite = hover; spriteinfo.EndLoc.x = hover->getwidth(); spriteinfo.EndLoc.y = hover->getheight(); }
	else
	{
		sprite = pressed; spriteinfo.EndLoc.x = pressed->getwidth(); spriteinfo.EndLoc.y = pressed->getheight();
	}
	

}

/**
 * ������ͨͼƬ.
 * 
 * \param [in] name ͼ������
 */
void Button::LoadNormalPicture(std::string name)
{
	normal = mainWorld.resourcePool->Fetch(name);
	if (!normal)return;
	spriteinfo.EndLoc = { normal->getwidth(), normal->getheight() };
}

/**
 * ������ͣ��ʱ���ͼƬ.
 * 
 * \param [in] name ͼ������
 */
void Button::LoadHoverPicture(std::string name)
{
	hover = mainWorld.resourcePool->Fetch(name);
	if (!hover)return;
	spriteinfo.EndLoc = { hover->getwidth(), hover->getheight() };
}

/**
 * ������갴��֮���ͼƬ.
 * 
 * \param [in] name ͼ������
 */
void Button::LoadPressedPicture(std::string name)
{
	pressed = mainWorld.resourcePool->Fetch(name);
	if (!pressed)return;
	spriteinfo.EndLoc = { pressed->getwidth(), pressed->getheight() };
}

/**
 * �Ƿ��������.
 * 
 * \return true�� false��
 */
bool Button::IsMousePressed()
{
	/*std::cout << InputComponent::IsMouseButtonClick() && IsMouseOn();*/
	return InputComponent::IsMouseButtonClick()&&IsMouseOn();
}

//��д�����±���
void Bar::Update()
{
	Widget::Update();
}

///ʵ�ֹ���
void Bar::Render()
{
	Vector2D pos;
	if (pospattern == LayoutPositionPattern::LeftTop)//���Ͻ�����ϵ
	pos = GetScreenPosition() + GetSize() / 2;
	else if (pospattern == LayoutPositionPattern::Center)//��������ϵ
	pos = GetScreenPosition();
	HDC dstDC = GetImageHDC();
	BLENDFUNCTION bf = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
	
	if (barback)
	{
		HDC srcDC = GetImageHDC(barback);
		int w = barback->getheight();		//ͼƬ��С
		int h = barback->getwidth();	
		AlphaBlend(srcDC, int(pos.x) - SizeBack.x / 2, int(pos.y )- SizeBack.y / 2, SizeBack.x, SizeBack.y, dstDC, 0, 0, w, h, bf);
	}


	if (barfront)
	{
		HDC srcDC = GetImageHDC(barback);
		Pair startpos, endpos;

		switch(direction)
		{
		case BarDirection::RightToLeft: startpos = { 0,0 }; endpos = { (int)(SizeFront.x * Percentage),SizeFront.y }; break;
		case BarDirection::LeftToRight: startpos = { (int)(SizeFront.x * (1 - Percentage)),0 }; endpos = { SizeFront.x,SizeFront.y }; break;
		case BarDirection::TopToBottom: startpos = { 0,(int)(SizeFront.y * (1 - Percentage)) }; endpos = { SizeFront.x,SizeFront.y }; break;
		case BarDirection::BottomToTop: startpos = { 0,0 }; endpos = { SizeFront.x,(int)(SizeFront.y * Percentage), }; break;
		default: startpos = { 0,0 }; endpos = { 0,0 }; break;
		}
		int w = endpos.x-startpos.x;		//ͼƬ��С
		int h = endpos.y-startpos.y;
		AlphaBlend(srcDC, int(pos.x) - SizeFront.x / 2, int(pos.y) - SizeFront.y / 2, SizeFront.x, SizeFront.y, dstDC, 0, 0, w, h, bf);
		
		if (barbutton)
		{
			AlphaBlend(srcDC, int(endpos.x) - SizeBottom.x / 2, int(endpos.x) - SizeBottom.y / 2, SizeBottom.x, SizeBottom.y, dstDC, 0, 0, w, h, bf);
		}
	}
	
}

/**
 * ���ؽ������ĵ�ͼ.
 *
 * \param [in] name ͼ������
 */
void Bar::LoadBarBackImg(std::string name)
{
	
	barback= mainWorld.resourcePool->Fetch(name);
	if (barback)
	SetSizeBack(barback->getwidth(),barback->getheight());
}

/**
 * ���ؽ������Ľ���ͼ��.
 *
 * \param [in] name ͼ������
 */
void Bar::LoadBarFrontImg(std::string name)
{
	barfront = mainWorld.resourcePool->Fetch(name);
	if (barfront)
		SetSizeBack(barfront->getwidth(), barfront->getheight());

}

/**
 * ���ؽ������İ�ťλ��ͼ��.
 *
 * \param [in] name ͼ������
 */
void Bar::LoadBarButtomImg(std::string name)
{
	barbutton = mainWorld.resourcePool->Fetch(name);
	if (barbutton)
		SetSizeBack(barbutton->getwidth(), barbutton->getheight());
}





