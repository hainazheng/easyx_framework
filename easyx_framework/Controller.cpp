#include "Controller.h"
#include"Camera.h"
#include"Math.h"


///创建输入交互组件和相机组件并进行绑定相机组件
Controller::Controller()
{
	camera = ConstructComponent<Camera>();
	camera->AttachTo(root);
	camera->SetMainCamera();			//绑定为世界中的我们要使用的相机
	
	inputcomponent = ConstructComponent<InputComponent>();
}

void Controller::SetupInputComponent(InputComponent* inputcomponent)
{
}

///@remark 这样实现了派生类间接访问基类的私有变量,派生类的形参inputcomponent就是基类的变量 用了虚函数表子类会代替父类的虚函数这里是父类私有类
void Controller::BeginPlay()
{
	
		Object::BeginPlay();
	SetupInputComponent(inputcomponent);		//这样实现了派生类间接访问基类的私有变量,派生类的形参inputcomponent就是基类的变量 用了虚函数表子类会代替父类的虚函数这里是父类私有类
	
}

Vector2D Controller::GetMousePosition() const
{
	return InputComponent::GetMousePosition() + mainWorld.mainCamera->GetWorldPosition() - Vector2D(getwidth(), getheight()) / 2;
}

bool Controller::IsMouseClick() const
{
	return InputComponent::IsMouseButtonClick();
}



///获取世界坐标系下鼠标所处位置最顶层的物体
Object* Controller::GetObjectUnderCursor()
{
	Vector2D pos = GetMousePosition();
	int x = (int)pos.x / 100; x = Math::Clamp(x, 0, RangeNumX-1);
	int y = (int)pos.y / 100; y = Math::Clamp(y, 0, RangeNumY-1);

	//只要发生接触返回最上面图层的指针
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

///实现相机可活动框限制
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

//获取世界坐标系下鼠标所处位置最顶层的物体信息（该物体必须拥有碰撞体）
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
