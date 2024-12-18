/*****************************************************************//**
 * \file   game.cpp
 * \brief  引擎、主入口
 * @brief  控制全局类的调度
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

/* 测试 */
//#include"TestObject.h"




/**
 * 引擎类
 *  用户不能控制.
 */
class Engine
{
public:
	///引擎初始化
	static void Init()
	{
		initgraph(WIN_WIDTH, WIN_HEIGHT);

		//系统Debug绘制参数
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL);
		setfillcolor(LIGHTBLUE);
		setlinecolor(LIGHTBLUE);
		
		//系统文字参数
		setbkmode(TRANSPARENT);
		settextstyle(25, 10, "Arial");
		settextcolor(WHITE);
		

		BeginBatchDraw();

		//初始化碰撞管理器
		mainWorld.resourcePool = ResourceManager::instance();
		mainWorld.resourcePool->Initialize();
		
		//初始化资源管理器
		mainWorld.collisionManager = new ColliderManager;
		mainWorld.collisionManager->Initialize();

		//初始化关卡管理器
		mainWorld.levelManager = LevelManager::instance();
		mainWorld.levelManager->Initialize();

		//初始化音频管理器
		mainWorld.audiomanager = AudioManager::instance();
		mainWorld.audiomanager->Initialize();

		//初始化日志类
		mainWorld.log = Log::instance();

		//初始化配置类
		mainWorld.m_cfg = Configurator::instance();
		mainWorld.m_cfg->Initialize();
		
		//初始化游戏世界
		mainWorld.CurrentLevel->BeginPlay();
		mainWorld.gameInstance = new GameInstance;

	
		mainWorld.FPSclock = new Timer;

		mainWorld.OverallClock = new Timer;

		//* 测试*/
		//TestObject* testobject = new TestObject;
		//mainWorld.GameObjects.insert(testobject);
	}

	///帧更新
	/// @param 上一帧间隔时间
	static void Tick(float deltaTime)
	{
		mainWorld.deltaTime = 0;		//帧间隔更新 这里的mainworld的时间是清零的时间，内部如果暂停了会减去这部分时间
		mainWorld.Update(deltaTime);		//这里的deltatime是上一帧的时间间隔
		mainWorld.Render();
		mainWorld.TransForm();
	}

	/**
	 * 更高频率的操作.
	 * @brief 默认一毫秒一次
	 */
	static void Tick_()
	{
		{
			std::lock_guard<std::mutex> lock(mainWorld.updateMutex);		//作用域结束就释放
			mainWorld.Input();
		}
		mainWorld.Update_();
	}

	/**
	 * 实现帧控制.
	 * 一般用户要自行通过全局静态变量
	 * bFrameLimitOn开启限制
	 * MaxFrame控制最大帧数
	 */
	static void Run()
	{
		//没执行项目之前的时间
		double lastTime = 0;
		while (true)
		{
			lastTime = GameStatics::GetTimeSeconds();
			Engine::Tick(mainWorld.deltaTime);
			if (bFrameLimitOn)
			{
				double interval = double(1) / double(MaxFrame) - (GameStatics::GetTimeSeconds() - lastTime) - 0.001;		//每一帧最少时间-这一帧执行持续时间-1秒千帧的单帧时间（最少的一帧时间）=这一帧剩下的时间
				if (interval > 0)std::this_thread::sleep_for(duration<double>(interval));
			}
			else _mm_pause();			//减少并行读取锁的数量 自旋锁
			//更新帧间隔时间
			mainWorld.deltaTime += float(GameStatics::GetTimeSeconds() - lastTime);
		}
	}

	///退出
	static void Exit()
	{
		delete mainWorld.gameInstance;
	}
};

//得到计时器发出的信号响应的操作
//void CALLBACK timecallback(UINT,UINT,DWORD_PTR,DWORD_PTR,DWORD_PTR)
//{
//	Engine::Tick();
//}

/**
 * 得到计时器发出的信号响应的操作.
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
	 * 类似信号槽机制，创建一个线程一直等待我们设置的时间就执行回调的操作timecallback_.
	 * 默认一毫秒一次
	 */
	timeSetEvent(1, 1, timecallback_, NULL, TIME_CALLBACK_FUNCTION | TIME_PERIODIC);

	Engine::Run();


	///主线程一直睡眠，分线程一直在运行
	Sleep(INFINITE);
	return 0;
}