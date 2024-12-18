/*****************************************************************//**
 * \file   game.cpp
 * \brief  ���桢�����
 * @brief  ����ȫ����ĵ���
 * \author zhn
 *********************************************************************/
#include"CoreMinimal.h"
#include<Windows.h>
#pragma comment(lib,"winmm.lib")
#include"Math.h"
#include"ResourceManager.h"
#include"BattleLevel.h"
#include"GameStatics.h"
#include"BattleController.h"
#include"ColliderManager.h"
#include"LevelManager.h"
#include"AudioManager.h"
#include"Log.h"
#include"Configurator.h"

#include <thread>
#include <immintrin.h>

/* ���� */
//#include"TestObject.h"




/**
 * ������
 *  �û����ܿ���.
 */
class Engine
{
public:
	///�����ʼ��
	static void Init()
	{
		initgraph(WIN_WIDTH, WIN_HEIGHT);

		//ϵͳDebug���Ʋ���
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL);
		setfillcolor(LIGHTBLUE);
		setlinecolor(LIGHTBLUE);
		
		//ϵͳ���ֲ���
		setbkmode(TRANSPARENT);
		settextstyle(25, 10, "Arial");
		settextcolor(WHITE);
		

		BeginBatchDraw();

		//��ʼ����ײ������
		mainWorld.resourcePool = ResourceManager::instance();
		mainWorld.resourcePool->Initialize();
		
		//��ʼ����Դ������
		mainWorld.collisionManager = new ColliderManager;
		mainWorld.collisionManager->Initialize();

		//��ʼ���ؿ�������
		mainWorld.levelManager = LevelManager::instance();
		mainWorld.levelManager->Initialize();

		//��ʼ����Ƶ������
		mainWorld.audiomanager = AudioManager::instance();
		mainWorld.audiomanager->Initialize();

		//��ʼ����־��
		mainWorld.log = Log::instance();

		//��ʼ��������
		mainWorld.m_cfg = Configurator::instance();
		mainWorld.m_cfg->Initialize();
		
		//��ʼ����Ϸ����
		mainWorld.CurrentLevel->BeginPlay();
		mainWorld.gameInstance = new GameInstance;

	
		mainWorld.FPSclock = new Timer;

		mainWorld.OverallClock = new Timer;

		//* ����*/
		//TestObject* testobject = new TestObject;
		//mainWorld.GameObjects.insert(testobject);
	}

	///֡����
	/// @param ��һ֡���ʱ��
	static void Tick(float deltaTime)
	{
		mainWorld.deltaTime = 0;		//֡������� �����mainworld��ʱ���������ʱ�䣬�ڲ������ͣ�˻��ȥ�ⲿ��ʱ��
		mainWorld.Update(deltaTime);		//�����deltatime����һ֡��ʱ����
		mainWorld.Render();
		mainWorld.TransForm();
	}

	/**
	 * ����Ƶ�ʵĲ���.
	 * @brief Ĭ��һ����һ��
	 */
	static void Tick_()
	{
		{
			std::lock_guard<std::mutex> lock(mainWorld.updateMutex);		//������������ͷ�
			mainWorld.Input();
		}
		mainWorld.Update_();
	}

	/**
	 * ʵ��֡����.
	 * һ���û�Ҫ����ͨ��ȫ�־�̬����
	 * bFrameLimitOn��������
	 * MaxFrame�������֡��
	 */
	static void Run()
	{
		//ûִ����Ŀ֮ǰ��ʱ��
		double lastTime = 0;
		while (true)
		{
			lastTime = GameStatics::GetTimeSeconds();
			Engine::Tick(mainWorld.deltaTime);
			if (bFrameLimitOn)
			{
				double interval = double(1) / double(MaxFrame) - (GameStatics::GetTimeSeconds() - lastTime) - 0.001;		//ÿһ֡����ʱ��-��һִ֡�г���ʱ��-1��ǧ֡�ĵ�֡ʱ�䣨���ٵ�һ֡ʱ�䣩=��һ֡ʣ�µ�ʱ��
				if (interval > 0)std::this_thread::sleep_for(duration<double>(interval));
			}
			else _mm_pause();			//���ٲ��ж�ȡ�������� ������
			//����֡���ʱ��
			mainWorld.deltaTime += float(GameStatics::GetTimeSeconds() - lastTime);
		}
	}

	///�˳�
	static void Exit()
	{
		delete mainWorld.gameInstance;
	}
};

//�õ���ʱ���������ź���Ӧ�Ĳ���
//void CALLBACK timecallback(UINT,UINT,DWORD_PTR,DWORD_PTR,DWORD_PTR)
//{
//	Engine::Tick();
//}

/**
 * �õ���ʱ���������ź���Ӧ�Ĳ���.
 */
void CALLBACK timecallback_(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR)
{
	Engine::Tick_();
}

int main()
{
	Engine::Init();
	
	/*timeSetEvent(DWORD(DELTA_TIME * 1000), 1, timecallback, NULL, TIME_CALLBACK_FUNCTION | TIME_PERIODIC);*/

	/**
	 * �����źŲۻ��ƣ�����һ���߳�һֱ�ȴ��������õ�ʱ���ִ�лص��Ĳ���timecallback_.
	 * Ĭ��һ����һ��
	 */
	timeSetEvent(1, 1, timecallback_, NULL, TIME_CALLBACK_FUNCTION | TIME_PERIODIC);

	Engine::Run();


	///���߳�һֱ˯�ߣ����߳�һֱ������
	Sleep(INFINITE);
	return 0;
}