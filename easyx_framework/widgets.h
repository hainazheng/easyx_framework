/*****************************************************************//**
 * \file   widgets.h
 * \brief  包含所有ui小部件的定义，提供了一些常见的ui小模型
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"
#include"unordered_map"
#include"VisualInterface.h"
#include"Delegate.h"
#include"World.h"


/**
 * 文字对齐格式.
 */
enum class CharactersPattern :uint8
{
	Left			///<左对齐 
	, Middle		///<居中 受总大小影响
	, Right			///<右对齐 受总大小影响
};

/**
 * 文字类.
 * @see 颜色值 Characters::TextColorMap
 * @see StrKit
 */
class Characters
{
	Vector2D pos = Vector2D(0, 0);		///<位置
	int row = 1;						///<行数
	int colume = 0;						///<最大的列数
	std::string texts = "";				///<文本内容
	
	int size = 3;						///<虚拟大小
	
	LPCTSTR type = LPCTSTR("楷体");		///<字体样式

	///颜色映射 
	/// @remark 可以在文本中写入相应名字实现修改相应文本颜色 格式:[名字 格式&相应值，颜色RGB]
	/// @remark 格式:[名字 格式&相应值，颜色RGB] 有新颜色想要但是没有的可以自己手动添加
	static std::unordered_map<std::string, COLORREF>TextColorMap;		

public:
	///获取宽度
	int GetWidth() { return colume * size * 3; }
	///获取高度
	int GetHeight() { return row * size * 6; }

	/**
	 * 设置文本样式.
	 * 
	 * \param [in] text 要输出的文本
	 * \param [in] size 虚拟大小
	 * \param [in] type	字体样式
	 */
	void SetCharacters(std::string text, int size = 3, LPCTSTR type = LPCTSTR("楷体"));

	/**
	 * 打印文本.
	 * 
	 * \param [in]  pos			位置
	 * \param [in] pattern		文字对齐格式
	 */
	void PrintCharacters(Vector2D pos, CharactersPattern pattern = CharactersPattern::Middle);

	///画文本的边框
	void debugline();
};

/**
 * 布局形式，相对位置改变
 * @brief 相对于绑定的父节点的大小.
 * 根据父节点的坐标系也会有所差别
 */
enum class LayoutPattern :uint8
{
	LeftTop,				///<左上位置
	MiddleTop,				///<中心顶部位置
	RightTop,				///<右上位置
	LeftMiddle,				///<左中心位置
	Center,					///<中心位置
	RightMiddle,			///<右中心位置
	LeftBottom,				///<左下位置
	MiddleBottom,			///<中心底部位置
	RightBottom				///<右下位置
};

/**
* 坐标系
 * @remark以那个布局坐标为基准作为扩大自己大小.注意坐标点
 */
enum class LayoutPositionPattern:uint8
{
	LeftTop,			///<左上角
	Center				///<默认中心坐标系

};

/**
 * 部件可视性.
 */
enum class UiPattern :uint8
{
	None,						///<既不可交互也不可视 相当于不存在
	VisibleOnly,				///<可视不可交互的模式
	VisibleAndInteractive		///<可是丙炔可交互
};

/**
 * 基础小部件 
 * @brief 结构类比component.
 */
class Widget :public base, public LayerInterface
{
	friend class Panel;
	friend class HorizontalPanel;

	//在哪一个区域
	Pair point_lt{ -1,-1 };		///<左上角所在的区域
	Pair point_rb{ -1,-1 };		///<右下角所在的区域

	///获取父节点座标坐标相对于相对位置的偏移量
	///@reamrk 受坐标系影响
	Vector2D GetLayoutOffset()const;
protected:
	Transform transform;		///<场景属性
	//实现基础小部件的绑定
	Widget* parent = nullptr;				///<父节点
	std::unordered_set<Widget*>children;	///<孩子容器
	Panel* attachedPanel = nullptr;			///<附着的容器面板

	///大小
	Vector2D size = {1,1};
	
	//自定义功能属性
	LayoutPattern layoutpattern;		///<布局形式
	UiPattern uipattern;				///<部件可视性
	LayoutPositionPattern pospattern=LayoutPositionPattern::Center;			///<坐标系

	Characters InfoText;			///<提示框
	bool bInfoBox = false;			///<是否开启文本显示
	///鼠标是否在点击的状态
	bool isUnderCursor()const;		
public:
	Widget():layoutpattern(LayoutPattern::LeftTop),uipattern(UiPattern::None),pospattern(LayoutPositionPattern::Center){}
	virtual ~Widget();

	virtual void Update();
	virtual void BeginPlay() {};
	virtual void EndPlay() {};
	virtual void Render() {};
	void ShowInfoBox() ;
	void DrawDebugRect();
	
	///设置布局形式
	void SetLayoutPattern(LayoutPattern pattern) { layoutpattern = pattern; }
	void SetUiPattern(UiPattern pattern);
	///设置坐标系
	void SetPosPattern(LayoutPositionPattern pattern) { pospattern = pattern; }
	///获取布局形式
	LayoutPattern GetLayoutPattern() const { return layoutpattern; }
	///获取部件可视性
	UiPattern GetUiPattern() const{ return uipattern; }
	///获取坐标系
	LayoutPositionPattern GetPosPattern() const{ return pospattern; }
	///是否开启提示框
	void EnableInfoBox(bool showinfo) { bInfoBox = showinfo; }
	///设置提示框内容
	/// @see Characters
	void SetInfoText(std::string text) { InfoText.SetCharacters(text); }

	virtual Vector2D GetSize()const;
	void SetSize(Vector2D size) { this->size = size; }

	///实现基础小部件的绑定
	void AttachTo(Widget*par) ;
	void DetachFrom(Widget* par) ;

	///获取相对屏幕绝对坐标
	 Vector2D GetScreenPosition()const;
	float GetScreenRotation()const;
	Vector2D GetScreenScale()const;

	///获取小部件相对位置
	Vector2D GetRelativePosition()const { return transform.position; }
	///获取小部件相对角度
	float GetRelativeRotation()const { return transform.rotation; }
	///获取小部件相对放缩比例
	Vector2D GetRelativeScale()const { return transform.scale; }

	///设置相对坐标
	void SetRelativePosition(Vector2D pos) { transform.position=pos; }
	///设置相对角度
	void SetRelativeRotation(float rotation) {  transform.rotation=rotation; }
	///设置小部件相对放缩比例
	void SetRelativeScale(Vector2D scale) { transform.scale=scale; }
};

/**
 * 容器面板基类.
 * @brief 既可以容纳UI类也可以容纳Widget类 是一种类图的关系
 * @remark 容纳小部件或UserInterface；当容纳UserInterface时,本质是容纳画布根部件
 */
class Panel:public Widget
{
	/**
	 * 调整大小.
	 * 不允许手动设置Panel的大小
	 * \param [in] size 大小
	 */
	void SetSize(Vector2D size) { this->size = size; }
protected:
	std::vector<Widget*> members;		///<存储部件
	std::vector<UserInterface*> members_ui;		///<本质是容纳画布根部件 一个整体

	Vector2D UnitSize;			///<单个存储部件大小

	/**
	 * 调节成员的开始位置
	 * @remark 子类重写 核心函数 
	 * @param [in] 部件成员.
	 * @param [in] 位置 从零开始
	 */
	virtual void AdjustMemberPosition(Widget* member,int32 index)=0;			
public:
	Panel();
	~Panel();

	virtual void Update()override;
	
	/**
	 * 设置标准容器部件大小.
	 * @param [in] 单个存储部件大小
	 */
	void SetUnitSize(Vector2D size);	

	/**
	 * 加入Widget的对象.
	 * @remark 用户创建之后首先使用函数
	 * @remark 后加入的会顶替原来index的 默认加入最后面
	 * @param [in] 部件成员
	 * @param [in] 位置
	 */
	void AddMember(Widget* member,int32 index=-1);
	/**
	 * 从panel中移除部件对象.
	 *  @param [in] 部件成员
	 */
	void Remove(Widget* member);							

	/**
	 * 加入widget和ui的对象 index是members的下标 .
	 * @remark 用户创建之后首先使用函数
	 * @param [in] UI类成员 实际是加入根部件
	 * @param [in] 位置
	 */
	void AddMember(UserInterface* member, int32 index = -1);	
	/**
	 * 从panel中移除UI对象.
	 *  @param [in] UI类成员
	 */
	void Remove(UserInterface* member);							

private:
	/**
	 * 调节成员尺寸到容器面板支持的尺寸.
	 *	@remark 不支持用户调节 内部函数
	 *   @param [in] 部件成员
	 */
	void AdjustMemberSizeToUnit(Widget* member);							
	//virtual Vector2D GetScreenPosition()const;
};

/**
 * 水平面板.
 * @brief 按照下标水平排列成员
 */
class HorizontalPanel :public Panel
{
	float spacing = 0;		///<成员中间空隙、间隙大小
	void AdjustMemberPosition(Widget* member, int32 index);
public:
	/**
	 * 设置成员间隙大小.
	 * 
	 * \param [in] spacing 间隔大小
	 */
	void SetSpacing(float spacing) { this->spacing = spacing; }
	Vector2D GetSize()const override;
	/*virtual Vector2D GetScreenPosition()const;*/
};

/**
 * 垂直面板.
 *  @brief 按照下标垂直排列成员
 */
class VerticalPanel :public Panel
{
	float spacing = 0;		///<成员中间空隙、间隙大小
	void AdjustMemberPosition(Widget* member, int32 index);
public:
	/**
	 * 设置成员间隙大小.
	 * 
	 * \param [in]spacing 间隔大小
	 */
	void SetSpacing(float spacing) { this->spacing = spacing; }
	Vector2D GetSize()const override;
};

/**
 * 网格面板.
 * @brief 按照下标按照网格行列排列成员 自适应
 * @remark 根据优先级 行适应或者列适应
 * @remark 行优先就是最大行数满足 列数根据数量来
 * @remark 列优先就是最大列数满足 行数根据数量来
 */
class GridPanel final:public Panel
{
	bool priority = false;	///<优先级 默认false是列优先，true是行优先
	float spacingx = 0;		///<成员水平距离
	float spacingy = 0;		///<成员垂直距离
	int row = 0;			///<总行数 是否有影响要看优先级
	int column=0;			///<总列数 是否有影响要看优先级
	void AdjustMemberPosition(Widget* member, int32 index);
public:
	/**
	 * 设置行数.
	 * @remark  是否有影响要看优先级
	 * \param [in] row 行数
	 */
	void SetRow(int row) { this->row = row; }
	/**
	 * 设置列数.
	 *  @remark  是否有影响要看优先级
	 * \param [in] col 列数
	 */
	void SetCol(int col) { this->column = col; }

	/**
	 * 设置成员间隙大小.
	 *
	 * \param [in] spacing 间隔大小
	 */
	void SetSpacing(Vector2D spacing) { spacingx = spacing.x; spacingy = spacing.y; }
	/**
	 * 设置成员间隙大小.
	 * 
	 * \param [in] x 行成员间距
	 * \param [in] y 列成员间距
	 */
	void SetSpacing(float x,float y) { spacingx = x; spacingy = y; }
	Vector2D GetSize()const override;
};

/**
 * 文本部件.
 * @see Characters
 */
class Text :public Widget
{
	Characters texts;
	CharactersPattern textpattern;		///<主要为了保留文本格式信息，好修改文本对齐形式
	const std::string* BindedText = nullptr;		///<绑定外部文本，进行更新，节省空间
public:
	Text():textpattern(CharactersPattern::Middle){}

	void Update();

	void Render()override;

	/**
	 * 设置文字对齐格式.
	 * 
	 * \param [in] pattern 文字对齐格式
	 */
	void SetPattern(CharactersPattern pattern) { textpattern = pattern; }

	/**
	 * 设置文本样式.
	 * @see Characters::SetCharacters()
	 * \param  [in] text 文本内容
	 * \param  [in] si   文字大小
	 * \param  [in] ty	字体
	 */
	void SetText(std::string text, int si = 3, LPCTSTR ty = LPCTSTR("楷体")) { texts.SetCharacters(text, si, ty); }

	/**
	 * 绑定文本.
	 * 
	 * \param [in] text 绑定的文本对象[引用]
	 */
	void BindText(const std::string& text) { BindedText = &text; }
};

/**
 * 图像部件.
 * @brief继承ImageInterface，支持图像和滤镜等操作
 * @see 类比SpriteRenderer类
 */
class Image :public Widget, public ImageInterface
{
public:
	void Update()override;
	void Render()override;

	///判断鼠标是否在图片上
	bool IsMouseOn();
};

///按钮委托(多播没有参数)
DECLARE_NO_PARAM_MULTI_DELEGATE_CLASS(ButtonDelegate)

/**
 * 按钮部件.
 */
class Button :public Image
{
	IMAGE* normal;			///<正常显示的图像
	IMAGE* hover;			///<正常悬停的图片
	IMAGE* pressed;			///<按下之后的图像
	
	bool isPressed=false;		///<是否处于按下状态
	bool ishover = true;		///<是否处于悬停状态
public:
	void Update()override;
	
	void LoadNormalPicture(std::string name);
	void LoadHoverPicture(std::string name);
	void LoadPressedPicture(std::string name);

	bool IsMousePressed();

	///鼠标按下去的开始事件
	ButtonDelegate OnMousePressedBegin;
	///鼠标按下去的结束事件
	ButtonDelegate OnMousePressedEnd;
	///鼠标悬停的开始事件
	ButtonDelegate OnMouseHoverBegin;
	///鼠标悬停的结束事件
	ButtonDelegate OnMouseHoverEnd;
};

/**
*  滑动条部件方向
 * @brief 滑动条的滑动方向 
 * @remark 默认从100% 减少.
 */
enum class BarDirection:uint8
{
	RightToLeft,		///<从右边开始减少 左到右
	LeftToRight,		///<从左边开始减少 右到左	
	TopToBottom,		///<从上边开始减少 上到下
	BottomToTop			///<从下边开始减少 下到上
};

/**
 *  滑动条部件
 * @brief 同时 支持渲染多个图像和位置.
 */
class Bar :public Widget
{
private:
	float Percentage=0;		///<进度比例0~1
	IMAGE* barback;			///<进度条的底图
	IMAGE* barfront;		///<进度条的进度图像
	IMAGE* barbutton;		///<进度条的按钮位置图像

	Pair SizeBack = { 0,0 };		///<进度条的底图大小
	Pair SizeFront = { 0,0 };		///<进度条的进度图像大小
	Pair SizeBottom = { 0,0 };		///<进度条的按钮位置图像大小
	BarDirection direction = BarDirection::RightToLeft;		///<默认方向是左到右

public:
	virtual void Update()override;
	virtual void Render()override;

	/**
	 * 设置比例.
	 * 
	 * \param [in] percentage 比例
	 */
	void SetPercent(float percentage) { this->Percentage = percentage; }
	/**
	 * 获取比例.
	 */
	float GetPercent()const { return Percentage ; }
	void LoadBarBackImg(std::string name);
	void LoadBarFrontImg(std::string name);
	void LoadBarButtomImg(std::string name);

	/**
	 * 设置进度条的底图大小.
	 * 
	 * \param [in] x 宽度
	 * \param [in] y 高度
	 */
	void SetSizeBack(int x, int y) { this->SizeBack = Pair(x, y); }
	/**
	 * 设置进度条的进度图像大小.
	 * 
	 * \param [in] x 宽度
	 * \param [in] y 高度
	 */
	void SetSizeFront(int x, int y) { this->SizeFront = Pair(x, y); }
	/**
	 * 设置进度条的按钮位置图像大小.
	 * 
	 * \param [in] x 宽度
	 * \param [in] y 高度
	 */
	void SetSizeBottom(int x, int y) { this->SizeBottom = Pair(x, y); }
	/**
	 * 设置进度条的底图大小.
	 * 
	 * \param [in] sizeback 底图大小 
	 */
	void SetSizeBack(Pair sizeback) { this->SizeBack = sizeback; }
	/**
	 * 设置进度条的进度图像大小.
	 *
	 * \param [in] sizeback 图像大小
	 */
	void SetSizeFront(Pair sizefront) { this->SizeFront = sizefront; }
	/**
	 * 设置进度条的按钮位置图像大小.
	 *
	 * \param [in] sizeback 图像大小
	 */
	void SetSizeBottom(Pair sizebottom) { this->SizeBottom = sizebottom; }

	/**
	 * 设置滑动条部件方向.
	 * 
	 * \param [in] direction 滑动方向
	 */
	void SetDirection(BarDirection direction) { this->direction = direction; }
};