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

///����һ��Ψһ��������
World mainWorld;



///ʵ������������������Ҫִ�еĲ���
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
	GameObjects_to_delete.clear();		//�����������ִ�л��������ڴ�����е�ʱ�����
	
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
	//�������������룩 ����ʵ�ֳ���һֱ����֮��ȫ������Ƶ��ִ��
	GameStatics::GetController()->PeekInfo();


	
}

///ʵ�ָ���Ƶ�ʵĲ���
void World::Update_()
{
	mainCamera->Calculate();
	

	/*CurrentLevel->Update();*/
	CurrentLevel->mainController->MouseTick();
}

///ʵ�����������Ⱦ
void World::Render()
{
	cleardevice();
	for (auto& obj : GameRenderers)
	{
		obj->Render();				//ÿ���̳�ͼ��ӿڵ���ض�����render����
	}
	for (auto& obj : GameUIs)
	{
		obj->ShowInfoBox();				//��ʾ���ѿ������ϲ�
	}
	/*Perlin p1;
	ImageToolkit::drawnoise(p1);*/

#ifdef _DEBUG
	Debug();
#endif // DEBUG


	/*Characters my;
	my.SetCharacters("$0���ɴ���\n$6is master",5);
	my.PrintCharacters({ 200,200 },CharactersPattern::Middle);
	my.debugline();*/

	FlushBatchDraw();
}

///ʵ�ָ���Ҳ���
void World::Input()
{
	//û�п����߾ͻᴴ�� ���ŵ����ж�
	GameStatics::GetController()->PeekInfo_();
}

void World::Debug()
{
	settextcolor(WHITE);
	settextstyle(20, 10, LPCTSTR("����"));
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
	number++;		//����

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
	//��������it�ǵ����� ������������
	//for (auto &it : GameColliders)
	//{
	//	////���¼�������ײ��������collider�еĴ��淢����ײ������
	//	//(*it)->Clear();

	//	if (it->GetCollisionMode() == ColliderMode::None)continue;
	//	//��ȡ��ײ������λ��������屾��
	//	Vector2D half;
	//	if (it->GetShape() == ColliderShape::Circle)
	//	{
	//		float a = Cast<CircleCollider>(it)->GetRadius();
	//		half = { a,a };
	//	}
	//	else half = Cast<BoxCollider>(it)->GetSize() / 2;

	//	//��ȡ��������ľ�������
	//	Vector2D pos = (it)->GetWorldPosition();
	//	//�ж������Ͻǵ����괦���ĸ�����
	//	pos -= half;
	//	int x = int(pos.x / 100); x = Math::Clamp(x, 0, 7);		//xֻ����[0,�������������ֵ]��
	//	int y = int(pos.y / 100); y = Math::Clamp(y, 0, 5);
	//	pos += half * 2;
	//	int x_1 = int(pos.x / 100); x_1 = Math::Clamp(x_1, 0, 7);
	//	int y_1 = int(pos.y / 100); y_1 = Math::Clamp(y_1, 0, 5);

	//	//�������������������� Բ���������ӷ������򣬲����ֻ�Ƿ�ֹ������㣬������뱾����������򲻻�Ӱ���������ֻ�ǻ�����
	//	for (int i = x; i <= x_1; ++i)
	//	{
	//		for (int j = y; j <= y_1; ++j)
	//			ColliderZones[i][j].insert(it);
	//	}
	//}
	
	//������������  ������������ײ������ײ���ڲ����ж���ײ����
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
	//ʵ���Լ��ڲ�����ײ�жϣ�ɾ���Ѿ�û����ײ�����
	for (auto& it : GameColliders) { it->Erase(); }
	
	{
		std::lock_guard<std::mutex> lock(updateMutex);		//��ֹ�Լ��ڲ�����clear
		for (auto& it : GameColliders_to_clear)it->Clear();
		GameColliders_to_clear.clear();
	}
	//��ײ���Ƴ�����

	//�������������룩
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

//ʵ�������Զ���˳��
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
