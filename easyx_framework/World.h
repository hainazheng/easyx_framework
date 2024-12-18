/*****************************************************************//**
 * \file   World.h
 * \brief  游戏世界类，所有基础类型组织逻辑
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"
#include"Object.h"
#include <mutex>

///实现渲染图层排序仿函数
/// 容器自定义顺序
/// @see 根据layer由小到大排序
struct LayerSort
{
	bool operator()(const class LayerInterface* a, const class LayerInterface* b)const;
};

///实现碰撞图层排序仿函数
/// 容器自定义顺序
/// @see 根据layer由小到大排序
struct ColliderSort
{
	bool operator()(const class  Collider* a, const class Collider* b)const;
};

///游戏世界
///@remark 存储全局所有对象，不允许外部继承修改
class World final			
{
	//友元 给类权限
	friend class GameStatics;
	friend class Object;
	friend class Timer;
	friend class Engine;
	friend class LayerInterface;
	friend void Object::Destroy();
	friend class ImageInterface;
	friend class Camera;
	friend class SpriteRenderer;
	friend class Animation;
	friend class Particle;
	friend class Level;

	friend class Collider;
	friend class CircleCollider;
	friend class BoxCollider;
	friend class Controller;
	friend class UserInterface;
	friend class Widget;
	friend class Button;
	friend class Bar;
	friend class LevelManager;
	friend class AudioPlayer;
	friend class AudioManager;
	friend class Log;
	friend class Configurator;


	std::mutex updateMutex;		///<互斥锁 防止某一个时刻耗时过多，导致下一次执行冲突 会一直等待防止顺序的未知

	///资源池
	///@remark 在初始化创建
	class ResourceManager* resourcePool = nullptr;		//在初始化创建

	///碰撞映射关系管理器
	/// @remark 在初始化创建 
	class ColliderManager* collisionManager = nullptr;	//在初始化创建

	/// 地图管理器
	///  @remark 在初始化创建
	class LevelManager* levelManager = nullptr;

	/// 全局音量管理器 
	class AudioManager* audiomanager = nullptr;

	/// 全局日志类
	class Log* log = nullptr;

	/// 全局配置器类
	class Configurator* m_cfg = nullptr;

	///全局计时器 
	/// @brief 记录框架开始了多久
	class Timer* OverallClock = nullptr;

	///帧间隔时间 两帧之间的时间
	/// @see 是上一帧的
	float deltaTime = 0;

	//场景对象，UI,计时器容器
	std::unordered_set<Object*> GameObjects;						///<存储所有游戏物体
	std::vector<Object*> GameObjects_to_add;						///<添加物体容器缓冲区 不会多次出现同一个对象，因为是用createobject来创建和加入
	std::unordered_set<Object*> GameObjects_to_delete;				///<设置对象的删除管理容器缓冲区，是为了防止发生删除之后迭代到空或者漏掉了对象
	std::unordered_set<class UserInterface*> GameUIs;				///<存储所有游戏UI
	std::unordered_set<class UserInterface*> GameUIs_to_delete;		///<删除游戏UI缓冲区
	std::unordered_set<class Timer*>GameTimers;						///<计时器容器

	//碰撞逻辑在gamecolliders每个碰撞组件遍历进行判断插入哪个区域(一个物体会有可能会有多个区域)，在每个区域内单独进行判定

	//渲染，碰撞计算容器
	std::set<class LayerInterface*, LayerSort>GameRenderers;				///< 存储所有支持渲染组件容器
	std::unordered_set<class Collider*> GameColliders;						///<底层哈希表不排序不能重复 存储所有碰撞组件容器
	std::unordered_set<class Collider*>GameColliders_to_clear;				///< 存删除碰撞组件缓冲区
	std::set<class Collider*, ColliderSort>ColliderZones[RangeNumX][RangeNumY];				///<存储每个区域存储的碰撞体 划分为100*100的网格，多于100的整数倍不能被除尽的部分单独做网格
	std::set<class LayerInterface*, LayerSort>UiDetectZones[RangeNumX][RangeNumY];			///<存储每个区域存储的ui部件进行交互



	///帧数的计时器
	class Timer* FPSclock = nullptr;

	//游戏单例对象
	Level* CurrentLevel = nullptr;					///<当前关卡
	class Controller* mainController = nullptr;
	GameInstance* gameInstance = nullptr;			///<游戏实例 想要存进去的单例 可以写进去

	///游戏场景的相机 也就是屏幕
	class Camera* mainCamera = nullptr;				

	//核心逻辑遍历
	///逻辑处理
	void Update(float deltaTime);

	///高精度计算
	void Update_();				//高精度计算

	///转换关卡
	void TransForm();

	///渲染
	void Render();

	///交互（轴输入）
	void Input();

	///Debug模式
	void Debug();

	///更新colliderzone 碰撞计算
	void ProcessColliderZones();

	///清空数据
	void ClearWorld();
};


//外部声明 全局唯一的全局世界类存放整个游戏需要的对象
extern World mainWorld;


