/*****************************************************************//**
 * \file   widgets.h
 * \brief  ��������uiС�����Ķ��壬�ṩ��һЩ������uiСģ��
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
 * ���ֶ����ʽ.
 */
enum class CharactersPattern :uint8
{
	Left			///<����� 
	, Middle		///<���� ���ܴ�СӰ��
	, Right			///<�Ҷ��� ���ܴ�СӰ��
};

/**
 * ������.
 * @see ��ɫֵ Characters::TextColorMap
 * @see StrKit
 */
class Characters
{
	Vector2D pos = Vector2D(0, 0);		///<λ��
	int row = 1;						///<����
	int colume = 0;						///<��������
	std::string texts = "";				///<�ı�����
	
	int size = 3;						///<�����С
	
	LPCTSTR type = LPCTSTR("����");		///<������ʽ

	///��ɫӳ�� 
	/// @remark �������ı���д����Ӧ����ʵ���޸���Ӧ�ı���ɫ ��ʽ:[���� ��ʽ&��Ӧֵ����ɫRGB]
	/// @remark ��ʽ:[���� ��ʽ&��Ӧֵ����ɫRGB] ������ɫ��Ҫ����û�еĿ����Լ��ֶ����
	static std::unordered_map<std::string, COLORREF>TextColorMap;		

public:
	///��ȡ���
	int GetWidth() { return colume * size * 3; }
	///��ȡ�߶�
	int GetHeight() { return row * size * 6; }

	/**
	 * �����ı���ʽ.
	 * 
	 * \param [in] text Ҫ������ı�
	 * \param [in] size �����С
	 * \param [in] type	������ʽ
	 */
	void SetCharacters(std::string text, int size = 3, LPCTSTR type = LPCTSTR("����"));

	/**
	 * ��ӡ�ı�.
	 * 
	 * \param [in]  pos			λ��
	 * \param [in] pattern		���ֶ����ʽ
	 */
	void PrintCharacters(Vector2D pos, CharactersPattern pattern = CharactersPattern::Middle);

	///���ı��ı߿�
	void debugline();
};

/**
 * ������ʽ�����λ�øı�
 * @brief ����ڰ󶨵ĸ��ڵ�Ĵ�С.
 * ���ݸ��ڵ������ϵҲ���������
 */
enum class LayoutPattern :uint8
{
	LeftTop,				///<����λ��
	MiddleTop,				///<���Ķ���λ��
	RightTop,				///<����λ��
	LeftMiddle,				///<������λ��
	Center,					///<����λ��
	RightMiddle,			///<������λ��
	LeftBottom,				///<����λ��
	MiddleBottom,			///<���ĵײ�λ��
	RightBottom				///<����λ��
};

/**
* ����ϵ
 * @remark���Ǹ���������Ϊ��׼��Ϊ�����Լ���С.ע�������
 */
enum class LayoutPositionPattern:uint8
{
	LeftTop,			///<���Ͻ�
	Center				///<Ĭ����������ϵ

};

/**
 * ����������.
 */
enum class UiPattern :uint8
{
	None,						///<�Ȳ��ɽ���Ҳ������ �൱�ڲ�����
	VisibleOnly,				///<���Ӳ��ɽ�����ģʽ
	VisibleAndInteractive		///<���Ǳ�Ȳ�ɽ���
};

/**
 * ����С���� 
 * @brief �ṹ���component.
 */
class Widget :public base, public LayerInterface
{
	friend class Panel;
	friend class HorizontalPanel;

	//����һ������
	Pair point_lt{ -1,-1 };		///<���Ͻ����ڵ�����
	Pair point_rb{ -1,-1 };		///<���½����ڵ�����

	///��ȡ���ڵ�����������������λ�õ�ƫ����
	///@reamrk ������ϵӰ��
	Vector2D GetLayoutOffset()const;
protected:
	Transform transform;		///<��������
	//ʵ�ֻ���С�����İ�
	Widget* parent = nullptr;				///<���ڵ�
	std::unordered_set<Widget*>children;	///<��������
	Panel* attachedPanel = nullptr;			///<���ŵ��������

	///��С
	Vector2D size = {1,1};
	
	//�Զ��幦������
	LayoutPattern layoutpattern;		///<������ʽ
	UiPattern uipattern;				///<����������
	LayoutPositionPattern pospattern=LayoutPositionPattern::Center;			///<����ϵ

	Characters InfoText;			///<��ʾ��
	bool bInfoBox = false;			///<�Ƿ����ı���ʾ
	///����Ƿ��ڵ����״̬
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
	
	///���ò�����ʽ
	void SetLayoutPattern(LayoutPattern pattern) { layoutpattern = pattern; }
	void SetUiPattern(UiPattern pattern);
	///��������ϵ
	void SetPosPattern(LayoutPositionPattern pattern) { pospattern = pattern; }
	///��ȡ������ʽ
	LayoutPattern GetLayoutPattern() const { return layoutpattern; }
	///��ȡ����������
	UiPattern GetUiPattern() const{ return uipattern; }
	///��ȡ����ϵ
	LayoutPositionPattern GetPosPattern() const{ return pospattern; }
	///�Ƿ�����ʾ��
	void EnableInfoBox(bool showinfo) { bInfoBox = showinfo; }
	///������ʾ������
	/// @see Characters
	void SetInfoText(std::string text) { InfoText.SetCharacters(text); }

	virtual Vector2D GetSize()const;
	void SetSize(Vector2D size) { this->size = size; }

	///ʵ�ֻ���С�����İ�
	void AttachTo(Widget*par) ;
	void DetachFrom(Widget* par) ;

	///��ȡ�����Ļ��������
	 Vector2D GetScreenPosition()const;
	float GetScreenRotation()const;
	Vector2D GetScreenScale()const;

	///��ȡС�������λ��
	Vector2D GetRelativePosition()const { return transform.position; }
	///��ȡС������ԽǶ�
	float GetRelativeRotation()const { return transform.rotation; }
	///��ȡС������Է�������
	Vector2D GetRelativeScale()const { return transform.scale; }

	///�����������
	void SetRelativePosition(Vector2D pos) { transform.position=pos; }
	///������ԽǶ�
	void SetRelativeRotation(float rotation) {  transform.rotation=rotation; }
	///����С������Է�������
	void SetRelativeScale(Vector2D scale) { transform.scale=scale; }
};

/**
 * ����������.
 * @brief �ȿ�������UI��Ҳ��������Widget�� ��һ����ͼ�Ĺ�ϵ
 * @remark ����С������UserInterface��������UserInterfaceʱ,���������ɻ���������
 */
class Panel:public Widget
{
	/**
	 * ������С.
	 * �������ֶ�����Panel�Ĵ�С
	 * \param [in] size ��С
	 */
	void SetSize(Vector2D size) { this->size = size; }
protected:
	std::vector<Widget*> members;		///<�洢����
	std::vector<UserInterface*> members_ui;		///<���������ɻ��������� һ������

	Vector2D UnitSize;			///<�����洢������С

	/**
	 * ���ڳ�Ա�Ŀ�ʼλ��
	 * @remark ������д ���ĺ��� 
	 * @param [in] ������Ա.
	 * @param [in] λ�� ���㿪ʼ
	 */
	virtual void AdjustMemberPosition(Widget* member,int32 index)=0;			
public:
	Panel();
	~Panel();

	virtual void Update()override;
	
	/**
	 * ���ñ�׼����������С.
	 * @param [in] �����洢������С
	 */
	void SetUnitSize(Vector2D size);	

	/**
	 * ����Widget�Ķ���.
	 * @remark �û�����֮������ʹ�ú���
	 * @remark �����Ļᶥ��ԭ��index�� Ĭ�ϼ��������
	 * @param [in] ������Ա
	 * @param [in] λ��
	 */
	void AddMember(Widget* member,int32 index=-1);
	/**
	 * ��panel���Ƴ���������.
	 *  @param [in] ������Ա
	 */
	void Remove(Widget* member);							

	/**
	 * ����widget��ui�Ķ��� index��members���±� .
	 * @remark �û�����֮������ʹ�ú���
	 * @param [in] UI���Ա ʵ���Ǽ��������
	 * @param [in] λ��
	 */
	void AddMember(UserInterface* member, int32 index = -1);	
	/**
	 * ��panel���Ƴ�UI����.
	 *  @param [in] UI���Ա
	 */
	void Remove(UserInterface* member);							

private:
	/**
	 * ���ڳ�Ա�ߴ絽�������֧�ֵĳߴ�.
	 *	@remark ��֧���û����� �ڲ�����
	 *   @param [in] ������Ա
	 */
	void AdjustMemberSizeToUnit(Widget* member);							
	//virtual Vector2D GetScreenPosition()const;
};

/**
 * ˮƽ���.
 * @brief �����±�ˮƽ���г�Ա
 */
class HorizontalPanel :public Panel
{
	float spacing = 0;		///<��Ա�м��϶����϶��С
	void AdjustMemberPosition(Widget* member, int32 index);
public:
	/**
	 * ���ó�Ա��϶��С.
	 * 
	 * \param [in] spacing �����С
	 */
	void SetSpacing(float spacing) { this->spacing = spacing; }
	Vector2D GetSize()const override;
	/*virtual Vector2D GetScreenPosition()const;*/
};

/**
 * ��ֱ���.
 *  @brief �����±괹ֱ���г�Ա
 */
class VerticalPanel :public Panel
{
	float spacing = 0;		///<��Ա�м��϶����϶��С
	void AdjustMemberPosition(Widget* member, int32 index);
public:
	/**
	 * ���ó�Ա��϶��С.
	 * 
	 * \param [in]spacing �����С
	 */
	void SetSpacing(float spacing) { this->spacing = spacing; }
	Vector2D GetSize()const override;
};

/**
 * �������.
 * @brief �����±갴�������������г�Ա ����Ӧ
 * @remark �������ȼ� ����Ӧ��������Ӧ
 * @remark �����Ⱦ�������������� ��������������
 * @remark �����Ⱦ�������������� ��������������
 */
class GridPanel final:public Panel
{
	bool priority = false;	///<���ȼ� Ĭ��false�������ȣ�true��������
	float spacingx = 0;		///<��Աˮƽ����
	float spacingy = 0;		///<��Ա��ֱ����
	int row = 0;			///<������ �Ƿ���Ӱ��Ҫ�����ȼ�
	int column=0;			///<������ �Ƿ���Ӱ��Ҫ�����ȼ�
	void AdjustMemberPosition(Widget* member, int32 index);
public:
	/**
	 * ��������.
	 * @remark  �Ƿ���Ӱ��Ҫ�����ȼ�
	 * \param [in] row ����
	 */
	void SetRow(int row) { this->row = row; }
	/**
	 * ��������.
	 *  @remark  �Ƿ���Ӱ��Ҫ�����ȼ�
	 * \param [in] col ����
	 */
	void SetCol(int col) { this->column = col; }

	/**
	 * ���ó�Ա��϶��С.
	 *
	 * \param [in] spacing �����С
	 */
	void SetSpacing(Vector2D spacing) { spacingx = spacing.x; spacingy = spacing.y; }
	/**
	 * ���ó�Ա��϶��С.
	 * 
	 * \param [in] x �г�Ա���
	 * \param [in] y �г�Ա���
	 */
	void SetSpacing(float x,float y) { spacingx = x; spacingy = y; }
	Vector2D GetSize()const override;
};

/**
 * �ı�����.
 * @see Characters
 */
class Text :public Widget
{
	Characters texts;
	CharactersPattern textpattern;		///<��ҪΪ�˱����ı���ʽ��Ϣ�����޸��ı�������ʽ
	const std::string* BindedText = nullptr;		///<���ⲿ�ı������и��£���ʡ�ռ�
public:
	Text():textpattern(CharactersPattern::Middle){}

	void Update();

	void Render()override;

	/**
	 * �������ֶ����ʽ.
	 * 
	 * \param [in] pattern ���ֶ����ʽ
	 */
	void SetPattern(CharactersPattern pattern) { textpattern = pattern; }

	/**
	 * �����ı���ʽ.
	 * @see Characters::SetCharacters()
	 * \param  [in] text �ı�����
	 * \param  [in] si   ���ִ�С
	 * \param  [in] ty	����
	 */
	void SetText(std::string text, int si = 3, LPCTSTR ty = LPCTSTR("����")) { texts.SetCharacters(text, si, ty); }

	/**
	 * ���ı�.
	 * 
	 * \param [in] text �󶨵��ı�����[����]
	 */
	void BindText(const std::string& text) { BindedText = &text; }
};

/**
 * ͼ�񲿼�.
 * @brief�̳�ImageInterface��֧��ͼ����˾��Ȳ���
 * @see ���SpriteRenderer��
 */
class Image :public Widget, public ImageInterface
{
public:
	void Update()override;
	void Render()override;

	///�ж�����Ƿ���ͼƬ��
	bool IsMouseOn();
};

///��ťί��(�ಥû�в���)
DECLARE_NO_PARAM_MULTI_DELEGATE_CLASS(ButtonDelegate)

/**
 * ��ť����.
 */
class Button :public Image
{
	IMAGE* normal;			///<������ʾ��ͼ��
	IMAGE* hover;			///<������ͣ��ͼƬ
	IMAGE* pressed;			///<����֮���ͼ��
	
	bool isPressed=false;		///<�Ƿ��ڰ���״̬
	bool ishover = true;		///<�Ƿ�����ͣ״̬
public:
	void Update()override;
	
	void LoadNormalPicture(std::string name);
	void LoadHoverPicture(std::string name);
	void LoadPressedPicture(std::string name);

	bool IsMousePressed();

	///��갴��ȥ�Ŀ�ʼ�¼�
	ButtonDelegate OnMousePressedBegin;
	///��갴��ȥ�Ľ����¼�
	ButtonDelegate OnMousePressedEnd;
	///�����ͣ�Ŀ�ʼ�¼�
	ButtonDelegate OnMouseHoverBegin;
	///�����ͣ�Ľ����¼�
	ButtonDelegate OnMouseHoverEnd;
};

/**
*  ��������������
 * @brief �������Ļ������� 
 * @remark Ĭ�ϴ�100% ����.
 */
enum class BarDirection:uint8
{
	RightToLeft,		///<���ұ߿�ʼ���� ����
	LeftToRight,		///<����߿�ʼ���� �ҵ���	
	TopToBottom,		///<���ϱ߿�ʼ���� �ϵ���
	BottomToTop			///<���±߿�ʼ���� �µ���
};

/**
 *  ����������
 * @brief ͬʱ ֧����Ⱦ���ͼ���λ��.
 */
class Bar :public Widget
{
private:
	float Percentage=0;		///<���ȱ���0~1
	IMAGE* barback;			///<�������ĵ�ͼ
	IMAGE* barfront;		///<�������Ľ���ͼ��
	IMAGE* barbutton;		///<�������İ�ťλ��ͼ��

	Pair SizeBack = { 0,0 };		///<�������ĵ�ͼ��С
	Pair SizeFront = { 0,0 };		///<�������Ľ���ͼ���С
	Pair SizeBottom = { 0,0 };		///<�������İ�ťλ��ͼ���С
	BarDirection direction = BarDirection::RightToLeft;		///<Ĭ�Ϸ���������

public:
	virtual void Update()override;
	virtual void Render()override;

	/**
	 * ���ñ���.
	 * 
	 * \param [in] percentage ����
	 */
	void SetPercent(float percentage) { this->Percentage = percentage; }
	/**
	 * ��ȡ����.
	 */
	float GetPercent()const { return Percentage ; }
	void LoadBarBackImg(std::string name);
	void LoadBarFrontImg(std::string name);
	void LoadBarButtomImg(std::string name);

	/**
	 * ���ý������ĵ�ͼ��С.
	 * 
	 * \param [in] x ���
	 * \param [in] y �߶�
	 */
	void SetSizeBack(int x, int y) { this->SizeBack = Pair(x, y); }
	/**
	 * ���ý������Ľ���ͼ���С.
	 * 
	 * \param [in] x ���
	 * \param [in] y �߶�
	 */
	void SetSizeFront(int x, int y) { this->SizeFront = Pair(x, y); }
	/**
	 * ���ý������İ�ťλ��ͼ���С.
	 * 
	 * \param [in] x ���
	 * \param [in] y �߶�
	 */
	void SetSizeBottom(int x, int y) { this->SizeBottom = Pair(x, y); }
	/**
	 * ���ý������ĵ�ͼ��С.
	 * 
	 * \param [in] sizeback ��ͼ��С 
	 */
	void SetSizeBack(Pair sizeback) { this->SizeBack = sizeback; }
	/**
	 * ���ý������Ľ���ͼ���С.
	 *
	 * \param [in] sizeback ͼ���С
	 */
	void SetSizeFront(Pair sizefront) { this->SizeFront = sizefront; }
	/**
	 * ���ý������İ�ťλ��ͼ���С.
	 *
	 * \param [in] sizeback ͼ���С
	 */
	void SetSizeBottom(Pair sizebottom) { this->SizeBottom = sizebottom; }

	/**
	 * ���û�������������.
	 * 
	 * \param [in] direction ��������
	 */
	void SetDirection(BarDirection direction) { this->direction = direction; }
};