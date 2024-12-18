#include "Controller.h"
#include"Camera.h"
#include"Math.h"


///�������뽻������������������а�������
Controller::Controller()
{
	camera = ConstructComponent<Camera>();
	camera->AttachTo(root);
	camera->SetMainCamera();			//��Ϊ�����е�����Ҫʹ�õ����
	
	inputcomponent = ConstructComponent<InputComponent>();
}

void Controller::SetupInputComponent(InputComponent* inputcomponent)
{
}

///@remark ����ʵ�����������ӷ��ʻ����˽�б���,��������β�inputcomponent���ǻ���ı��� �����麯�����������游����麯�������Ǹ���˽����
void Controller::BeginPlay()
{
	
		Object::BeginPlay();
	SetupInputComponent(inputcomponent);		//����ʵ�����������ӷ��ʻ����˽�б���,��������β�inputcomponent���ǻ���ı��� �����麯�����������游����麯�������Ǹ���˽����
	
}

Vector2D Controller::GetMousePosition() const
{
	return InputComponent::GetMousePosition() + mainWorld.mainCamera->GetWorldPosition() - Vector2D(getwidth(), getheight()) / 2;
}

bool Controller::IsMouseClick() const
{
	return InputComponent::IsMouseButtonClick();
}



///��ȡ��������ϵ���������λ����������
Object* Controller::GetObjectUnderCursor()
{
	Vector2D pos = GetMousePosition();
	int x = (int)pos.x / 100; x = Math::Clamp(x, 0, RangeNumX-1);
	int y = (int)pos.y / 100; y = Math::Clamp(y, 0, RangeNumY-1);

	//ֻҪ�����Ӵ�����������ͼ���ָ��
	if (!mainWorld.ColliderZones[x][y].empty())
	{
		for (auto it = mainWorld.ColliderZones[x][y].rbegin(); it != mainWorld.ColliderZones[x][y].rend(); ++it)
		{
			if ((*it)->IsMouseOver()) { return (*it)->pOwner; }
		}
	}
	return nullptr;
}

void Controller::DrawDebugPosition()const
{
	Vector2D pos = Vector2D(getwidth() / 2, getheight() / 2);
	/*std::cout << GetWorldPosition() << std::endl;*/
	outtextxy((int)pos.x, (int)pos.y,
		LPCTSTR((GetWorldPosition()+ Vector2D(getwidth() / 2, getheight() / 2)).To_String().c_str()));
}

///ʵ������ɻ������
void Controller::Update(float deltaTime)
{
	Vector2D pos = GetWorldPosition();
	if (camera->frame.left != 0 || camera->frame.right != 0 || camera->frame.bottom != 0 || camera->frame.top != 0)
	{
		if (pos.x < camera->frame.left)AddPosition({ -pos.x + camera->frame.left ,0 });
		if (pos.x + WIN_WIDTH > camera->frame.right)AddPosition({ -(pos.x + WIN_WIDTH - camera->frame.right),0 });
		if (pos.y + WIN_HEIGHT > camera->frame.bottom)AddPosition({ 0,-(camera->frame.bottom + WIN_HEIGHT - camera->frame.right) });
		if (pos.y < camera->frame.top)AddPosition({ 0,-pos.y + camera->frame.top });
	}
}

//��ȡ��������ϵ���������λ������������Ϣ�����������ӵ����ײ�壩
HitResult Controller::GetHitResultUnderCursor()
{
	Vector2D pos = GetMousePosition();
	int x = Math::Clamp(int(pos.x) / 100, 0, RangeNumX-1);
	int y = Math::Clamp(int(pos.y) / 100, 0, RangeNumY-1);

	if (!mainWorld.ColliderZones[x][y].empty())
	{
		for (auto it = mainWorld.ColliderZones[x][y].rbegin(); it != mainWorld.ColliderZones[x][y].rend(); ++it)
		{
			if ((*it)->IsMouseOver())return HitResult(pos, { 0,0 }, (*it)->pOwner, *it);
		}
	}
	return HitResult();
}

void Controller::EnableInput(bool enable)
{
	InputComponent::EnableInput(enable);
}

void Controller::MouseTick()
{
	inputcomponent->MouseTick();
}

void Controller::SetRectFrame(const Rect& frame)
{
	camera->SetRectFrame(frame);
}

//void Controller::UserClickMove()
//{
//	Vector2D pos = GetMousePosition();
//	
//		if (msg.message == WM_MOUSEMOVE)
//		{
//			putstone(msg.x, msg.y, 15, 2560);
//		}
//		else if (msg.message == WM_LBUTTONDOWN)
//		{
//			putstone(msg.x, msg.y, 30, 2560);
//		}
//
//}
