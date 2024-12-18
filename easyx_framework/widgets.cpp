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
	//初始化
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
	//每一行的数据
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

///析构函数
///从mainworld容器区域中删去类
Widget::~Widget()
{
	for (int i = point_lt.x; i <= point_rb.x; i++)for (int j = point_lt.y; j <= point_rb.y; j++)mainWorld.UiDetectZones[i][j].erase(this);
}

///插入世界容器中进行一些交互处理
/// 每一帧gengxin
void Widget::Update()
{
	//实现交互
	if (uipattern != UiPattern::VisibleAndInteractive)return;
	Vector2D pos(0,0); 
	if (pospattern == LayoutPositionPattern::Center)
		pos = GetScreenPosition() - GetSize() / 2;	//中心坐标系 获取左上角点坐标
	else if (pospattern == LayoutPositionPattern::LeftTop)
		pos = GetScreenPosition();		//左上角坐标系
	Pair newpoint(Math::Clamp(int(pos.x/100),0,7),Math::Clamp(int(pos.y/100),0,5));
	pos += GetSize();
	Pair newpoint_1(Math::Clamp(int(pos.x / 100), 0, 7), Math::Clamp(int(pos.y/100),0,5));
	if (newpoint == point_lt && newpoint_1 == point_rb)return;

	if (point_lt != Pair(-1, -1))for (int i = point_lt.x; i <= point_rb.x; i++)for (int j = point_lt.y; j <= point_rb.y; j++)mainWorld.UiDetectZones[i][j].erase(this);
	point_lt = newpoint; point_rb = newpoint_1;
	for (int i = point_lt.x; i <= point_rb.x; i++)for (int j = point_lt.y; j <= point_rb.y; j++)mainWorld.UiDetectZones[i][j].insert(this);

}

///显示提示框
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

///绘制部件的边框
void Widget::DrawDebugRect()
{
	setlinecolor(BLUE);
	Vector2D pos = { 0,0 };
	if (pospattern == LayoutPositionPattern::Center)
		pos = GetScreenPosition() - GetSize() / 2;	//中心坐标系 获取左上角点坐标
	else if (pospattern == LayoutPositionPattern::LeftTop)
		pos = GetScreenPosition();		//左上角坐标系
	int left = int(pos.x), top = int(pos.y);
	pos += GetSize();
	int right = int(pos.x), bottom = int(pos.y);
	rectangle(left, top, right, bottom);
}

///设置部件可视性
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

///获取实际大小
Vector2D Widget::GetSize() const
{
	return size*GetScreenScale();			//大小*比例
}

void Widget::AttachTo(Widget* par)
{
	if (par)
	{
		par->children.insert(this);
		parent = par;
	}
}


///解除基础小部件的绑定
/// @remark !!!用户慎用
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

///获取相对屏幕坐标系的角度
float Widget::GetScreenRotation() const
{
	if (parent)return GetRelativeRotation() + parent->GetScreenRotation();
	else return GetRelativeRotation();
}

///获取绝对放缩比例
Vector2D Widget::GetScreenScale() const
{
	if (parent)return GetRelativeScale() * parent->GetScreenScale();
	else return GetRelativeScale();
}

Panel::Panel()
{
}

///析构函数
/// @brief 把所有绑定的成员都隐藏
Panel::~Panel()
{
	if (members_ui.empty())return;
	for (auto& ui : members_ui)
	{
		ui->RemoveFromViewport();
	}
}

///更新绑定成员位置
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
	members.erase(std::remove(members.begin(),members.end(),member),members.end());			//vector删除remove返回的是没有被删除的最后一个元素的位置
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
	members.erase(std::remove(members.begin(), members.end(), member->root), members.end());			//vector删除remove返回的是没有被删除的最后一个元素的位置
	members_ui.erase(std::remove(members_ui.begin(), members_ui.end(), member), members_ui.end());
	member->root->attachedPanel = nullptr;
}

///不改变原本大小，改变比例，好能够恢复原本大小 只要调节回(1,1)就行
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

///水平排列更新坐标
void HorizontalPanel::AdjustMemberPosition(Widget* member, int32 index)
{
	if (index<0)return;
	Vector2D pos = Vector2D(index * (spacing + UnitSize.x), 0);
	if(member->GetPosPattern() == LayoutPositionPattern::Center)
	pos = pos+ UnitSize * 0.5f;			//把每一张图转换成中心坐标位置

	member->SetRelativePosition(pos);
	
}

///实际大小=(成员数量*UnitSizex大小,UnitSizey大小)+((数量-1)*空隙,UnitSizey大小)
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

///垂直排列更新坐标
void VerticalPanel::AdjustMemberPosition(Widget* member, int32 index)
{
	if (index<0)return;
	Vector2D pos = Vector2D(0, index * (spacing + UnitSize.y));
	if (member->GetPosPattern() == LayoutPositionPattern::Center)
	pos = pos + UnitSize * 0.5f;
	member->SetRelativePosition(pos);
}

///实际大小=(UnitSizex大小,成员数量*UnitSizex大小)+(UnitSizex大小,(数量-1)*空隙)
Vector2D VerticalPanel::GetSize() const
{
	return (members.empty()) ? Vector2D(0, 0) : Vector2D(UnitSize.x, members.size() * (spacing + UnitSize.y) - spacing);
}

///行列排列更新坐标
void GridPanel::AdjustMemberPosition(Widget* member, int32 index)
{
	if (!index)return;
	//自适应
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

///更新文本坐标和文本样式
void Text::Update()
{
	Widget::Update();
	if(BindedText)texts.SetCharacters(*BindedText);
	if(GetPosPattern()==LayoutPositionPattern::Center)
	size = Vector2D(float(texts.GetWidth()), float(texts.GetHeight()));

}

///渲染文本
void Text::Render()
{
	//2024.11.18 DEBUG 文本没有按照中心坐标进行渲染
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

	//旋转的时候滤镜更新
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
	//渲染默认pos为中心坐标

	/*std::cout << "joinui" << std::endl;*/
	Vector2D pos = { 0,0 };
	if (pospattern == LayoutPositionPattern::Center)
		pos = GetScreenPosition();	//中心坐标系 获取左上角点坐标
	else if (pospattern == LayoutPositionPattern::LeftTop)
		pos = GetScreenPosition() + GetSize() / 2;;		//左上角坐标系

		//判断是否有对图形进行旋转操作
		IMAGE* img = copy ? copy : sprite;

		HDC dstDC = GetImageHDC();				//工作平面的图片句柄
		HDC srcDC = GetImageHDC(img);			//要传递的图片句柄
		int w = spriteinfo.EndLoc.x -spriteinfo.StartLoc.x;			//IMAGE类型自带获取对象的宽度
		int h = spriteinfo.EndLoc.y - spriteinfo.StartLoc.y;		//IMAGE类型自带获取对象的高度

		int dw = int(GetSize().x * w / img->getwidth());
		int dh = int(GetSize().y * h / img->getheight());
		
		/*std::cout << (int)pos.x - dw / 2 <<" "<< (int)pos.y - dh / 2 << std::endl;*/

		BLENDFUNCTION bf = { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA };				//设置透明度，成员全是BYTE类型
		//是否添加了滤镜
		if (bFilterEnable && filter && FilterLayer.size() > 0)
		{
			HDC srcDC_filter = GetImageHDC(filter);
			AlphaBlend(dstDC, (int)pos.x - GetSize().x / 2, (int)pos.y - GetSize().y / 2, dw, dh, srcDC_filter, (int)spriteinfo.StartLoc.x, (int)spriteinfo.StartLoc.y, w, h, bf);			//相当于putimage 粘贴到相应的工作平面

		}
		else
			AlphaBlend(dstDC, (int)pos.x - GetSize().x / 2, (int)pos.y - GetSize().y / 2, dw, dh, srcDC, (int)spriteinfo.StartLoc.x, (int)spriteinfo.StartLoc.y, w, h, bf);			//相当于putimage 粘贴到相应的工作平面
	
}

//鼠标是否在部件上
bool Image::IsMouseOn()
{

	return isUnderCursor();
}

///实现功能
void Button::Update()
{
	Image::Update();
	//判断鼠标事件
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

	//如果其他图片都还没有
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
 * 加载普通图片.
 * 
 * \param [in] name 图像名字
 */
void Button::LoadNormalPicture(std::string name)
{
	normal = mainWorld.resourcePool->Fetch(name);
	if (!normal)return;
	spriteinfo.EndLoc = { normal->getwidth(), normal->getheight() };
}

/**
 * 加载悬停的时候的图片.
 * 
 * \param [in] name 图像名字
 */
void Button::LoadHoverPicture(std::string name)
{
	hover = mainWorld.resourcePool->Fetch(name);
	if (!hover)return;
	spriteinfo.EndLoc = { hover->getwidth(), hover->getheight() };
}

/**
 * 加载鼠标按下之后的图片.
 * 
 * \param [in] name 图像名字
 */
void Button::LoadPressedPicture(std::string name)
{
	pressed = mainWorld.resourcePool->Fetch(name);
	if (!pressed)return;
	spriteinfo.EndLoc = { pressed->getwidth(), pressed->getheight() };
}

/**
 * 是否点击了鼠标.
 * 
 * \return true是 false否
 */
bool Button::IsMousePressed()
{
	/*std::cout << InputComponent::IsMouseButtonClick() && IsMouseOn();*/
	return InputComponent::IsMouseButtonClick()&&IsMouseOn();
}

//重写来更新比例
void Bar::Update()
{
	Widget::Update();
}

///实现功能
void Bar::Render()
{
	Vector2D pos;
	if (pospattern == LayoutPositionPattern::LeftTop)//左上角坐标系
	pos = GetScreenPosition() + GetSize() / 2;
	else if (pospattern == LayoutPositionPattern::Center)//中心坐标系
	pos = GetScreenPosition();
	HDC dstDC = GetImageHDC();
	BLENDFUNCTION bf = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
	
	if (barback)
	{
		HDC srcDC = GetImageHDC(barback);
		int w = barback->getheight();		//图片大小
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
		int w = endpos.x-startpos.x;		//图片大小
		int h = endpos.y-startpos.y;
		AlphaBlend(srcDC, int(pos.x) - SizeFront.x / 2, int(pos.y) - SizeFront.y / 2, SizeFront.x, SizeFront.y, dstDC, 0, 0, w, h, bf);
		
		if (barbutton)
		{
			AlphaBlend(srcDC, int(endpos.x) - SizeBottom.x / 2, int(endpos.x) - SizeBottom.y / 2, SizeBottom.x, SizeBottom.y, dstDC, 0, 0, w, h, bf);
		}
	}
	
}

/**
 * 加载进度条的底图.
 *
 * \param [in] name 图像名字
 */
void Bar::LoadBarBackImg(std::string name)
{
	
	barback= mainWorld.resourcePool->Fetch(name);
	if (barback)
	SetSizeBack(barback->getwidth(),barback->getheight());
}

/**
 * 加载进度条的进度图像.
 *
 * \param [in] name 图像名字
 */
void Bar::LoadBarFrontImg(std::string name)
{
	barfront = mainWorld.resourcePool->Fetch(name);
	if (barfront)
		SetSizeBack(barfront->getwidth(), barfront->getheight());

}

/**
 * 加载进度条的按钮位置图像.
 *
 * \param [in] name 图像名字
 */
void Bar::LoadBarButtomImg(std::string name)
{
	barbutton = mainWorld.resourcePool->Fetch(name);
	if (barbutton)
		SetSizeBack(barbutton->getwidth(), barbutton->getheight());
}





