#include"World.h"
#include"GameStatics.h"
#include"LevelManager.h"
#include"Timer.h"
#include"VisualInterface.h"
#include"Camera.h"
#include"Collider.h"
#include"Controller.h"
#include"UserInterface.h"
#include"Math.h"
#include"Component.h"
#include"SceneComponent.h"
#include"Object.h"

int Object::tag = 0;

///定义一个唯一的世界类
World mainWorld;



///实现世界类容器所有想要执行的操作
void World::Update(float deltaTime)
{
	ProcessColliderZones();

	CurrentLevel->Update(deltaTime);
	for (auto& obj : GameObjects)
	{
		obj->Update(deltaTime);
	}
	{
		std::lock_guard<std::mutex> lock(updateMutex);
		for (auto& obj : GameObjects_to_add)
		{
			GameObjects.insert(obj);
			obj->BeginPlay();
		}
		GameObjects_to_add.clear();
		for (auto& obj : GameObjects_to_delete)
		{
			GameObjects.erase(obj);
			obj->EndPlay();
			delete obj;
		}
	}
	GameObjects_to_delete.clear();		//如果有物体在执行回收物体内存过程中的时候加入
	
	for (auto& obj : GameUIs)
	{
		obj->Update(deltaTime);
	}
	{
		std::lock_guard<std::mutex> lock(updateMutex);
		for (auto& obj : GameUIs_to_delete)
		{
			GameUIs.erase(obj);
			delete obj;
		}
		GameUIs_to_delete.clear();
		for (auto& obj : GameTimers)
		{
			obj->Execute();
		}
	}
	//交互处理（点输入） 这里实现除了一直按着之外全部正常频率执行
	GameStatics::GetController()->PeekInfo();


	
}

///实现更高频率的操作
void World::Update_()
{
	mainCamera->Calculate();
	

	/*CurrentLevel->Update();*/
	CurrentLevel->mainController->MouseTick();
}

///实现世界类的渲染
void World::Render()
{
	cleardevice();
	for (auto& obj : GameRenderers)
	{
		obj->Render();				//每个继承图层接口的类必定会有render函数
	}
	for (auto& obj : GameUIs)
	{
		obj->ShowInfoBox();				//显示提醒框在最上层
	}
	/*Perlin p1;
	ImageToolkit::drawnoise(p1);*/

#ifdef _DEBUG
	Debug();
#endif // DEBUG


	/*Characters my;
	my.SetCharacters("$0海纳大王\n$6is master",5);
	my.PrintCharacters({ 200,200 },CharactersPattern::Middle);
	my.debugline();*/

	FlushBatchDraw();
}

///实现跟玩家操作
void World::Input()
{
	//没有控制者就会创建 按着单独判断
	GameStatics::GetController()->PeekInfo_();
}

void World::Debug()
{
	settextcolor(WHITE);
	settextstyle(20, 10, LPCTSTR("宋体"));
	for (auto& obj : GameColliders)
		obj->DrawDebugLine();
	for (auto obj : GameObjects)
		obj->DrawDebugPosition();
	for (auto& obj : GameUIs)
		obj->DrawDebugRect();
	/*mainController->DrawDebugPosition();*/
	/*setbkmode(TRANSPARENT);*/
	static int FPS = 0;
	static int number = 0;
	number++;		//计数

	outtextxy(0, 0, LPCTSTR(((std::string("FPS: ") + std::to_string(FPS)).c_str())));
	if (FPSclock->GetDelay() >= 1.0)
	{
		FPS = number;
		FPSclock->ReSet();
		number = 0;
	}

}

void World::ProcessColliderZones()
{
	/*for (auto& arr_i : ColliderZones)
	{
		for (auto& arr_j : arr_i)
		{
			arr_j.clear();
		}
	}*/
	//迭代遍历it是迭代器 插入区域容器
	//for (auto &it : GameColliders)
	//{
	//	////重新计算有碰撞的组件清空collider中的储存发生碰撞的容器
	//	//(*it)->Clear();

	//	if (it->GetCollisionMode() == ColliderMode::None)continue;
	//	//获取碰撞体中心位置相对物体本身
	//	Vector2D half;
	//	if (it->GetShape() == ColliderShape::Circle)
	//	{
	//		float a = Cast<CircleCollider>(it)->GetRadius();
	//		half = { a,a };
	//	}
	//	else half = Cast<BoxCollider>(it)->GetSize() / 2;

	//	//获取本身物体的绝对坐标
	//	Vector2D pos = (it)->GetWorldPosition();
	//	//判断最左上角的坐标处在哪个区域
	//	pos -= half;
	//	int x = int(pos.x / 100); x = Math::Clamp(x, 0, 7);		//x只能在[0,横坐标区间最大值]、
	//	int y = int(pos.y / 100); y = Math::Clamp(y, 0, 5);
	//	pos += half * 2;
	//	int x_1 = int(pos.x / 100); x_1 = Math::Clamp(x_1, 0, 7);
	//	int y_1 = int(pos.y / 100); y_1 = Math::Clamp(y_1, 0, 5);

	//	//迭代遍历加入区域容器 圆计算的是外接方的区域，插入的只是防止多余计算，就算插入本身不在这个区域不会影响最后结果，只是会多计算
	//	for (int i = x; i <= x_1; ++i)
	//	{
	//		for (int j = y; j <= y_1; ++j)
	//			ColliderZones[i][j].insert(it);
	//	}
	//}
	
	//迭代区域容器  更新物体间的碰撞插入碰撞的内部的判定碰撞容器
	for (auto& arr_i : ColliderZones)
	{
		
		for (auto& arr_j : arr_i)
		{
			if (!arr_j.empty())
			{
				/*for (auto me = arr_j.begin(); me != arr_j.end(); me++)
				{
					for (auto an = arr_j.begin(); an != arr_j.end(); an++)
					{
						if (me != an)(*me)->Insert(*an);
					}
				}*/
				
				for (auto& me : arr_j)for (auto& he : arr_j) if (he != me) me->Insert(he);
			}
		}
	}
	//实现自己内部的碰撞判断，删除已经没有碰撞的组件
	for (auto& it : GameColliders) { it->Erase(); }
	
	{
		std::lock_guard<std::mutex> lock(updateMutex);		//防止自己内部调用clear
		for (auto& it : GameColliders_to_clear)it->Clear();
		GameColliders_to_clear.clear();
	}
	//碰撞体移除更新

	//交互处理（点输入）
	GameStatics::GetController()->PeekInfo();
}

void World::TransForm()
{
	levelManager->RefreshLevel();
}

void World::ClearWorld()
{
	for (auto& obj : GameObjects)delete obj;
	for (auto& obj : GameUIs)delete obj;

	GameObjects.clear();
	GameObjects_to_add.clear();
	GameObjects_to_delete.clear();
	GameUIs.clear();
	GameUIs_to_delete.clear();
	GameColliders.clear();
	GameColliders_to_clear.clear();
	GameTimers.clear();
	GameRenderers.clear();
	for (auto& arr_i : ColliderZones)for (auto& arr_j : arr_i)arr_j.clear();
}

//实现容器自定义顺序
bool LayerSort::operator()(const LayerInterface* a, const LayerInterface* b) const
{
	if (a->GetLayer() == b->GetLayer())return a < b;
	else
	{
		return a->GetLayer() < b->GetLayer();
	}
}

bool ColliderSort::operator()(const Collider* a, const Collider* b) const
{

	if (a->GetLayer() == b->GetLayer())return a < b;
	else
	{
		return a->GetLayer() < b->GetLayer();
	}
}
