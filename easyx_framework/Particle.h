/*****************************************************************//**
 * \file   Particle.h
 * \brief  2D粒子组件
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"
#include"VisualInterface.h"
#include <chrono>
#include<deque>
#include"SceneComponent.h"

using std::chrono::steady_clock;
using std::chrono::duration;
using std::chrono::time_point;

///粒子发射的形式
enum class ParticlePattern:uint8
{
	Center,			///<中心放射状
	Line,			///<线性放射状 只能在确定的角度生成线,粒子跟线成90度
};


///记录粒子信息类
struct ParticleInfo
{
	Vector2D offset;		///<用于记录粒子相对于粒子创建器中心位置
	int index=0;			///<当前帧索引 这个粒子是哪张图片
	Vector2D velocity;		///<速度
	time_point<steady_clock> lasttime;		///<粒子创建时间
	float alpha = 255.f;			///<透明度 实现粒子逐渐透明效果
	float size = 1.f;				///<大小
	ParticleInfo(){ lasttime = steady_clock::now(); }
};





/**
 * 粒子组件 
 * @remark 生产和管理粒子销毁.
 * @remark 跟渲染组件差不多 但是需要动画图片和帧数 来实现随机发射不同帧数的图片
 */
class Particle :public SceneComponent, public LayerInterface, public ImageInterface
{
	std::deque<ParticleInfo> particle;		///<粒子容器 用于管理生产出来的每一个粒子
	int capacity = 0;			///<粒子容量

	//float speed = 1.f;			//描述速度快慢的标量
	float maxSpeed = 1.f;			///<粒子最大初速度
	float minSpeed = 1.f;			///<粒子最小初速度
	float gravity = 9.8f;			///<重力加速度
	float lifetime = 1.f;			///<单个粒子的生命周期
	bool isLoop = true;				///<是否循环生产
	float Interval=0.1f;			///<生产间隔
	///粒子逐渐消失的时间 
	///@brief 主要是实现过渡效果
	float fadingtime = 0;			


	IMAGE** images=nullptr;					///<动画帧数组
	int number = 0;							///<动画粒子总帧数
	time_point<steady_clock> lasttime;		///<记录生产的时间点
	Vector2D sizeRange;						///<大小范围（倍率系数）

	ParticlePattern pattern = ParticlePattern::Center;		////<粒子放射状模式
	Vector2D unitVector = Vector2D(0, 1);		///<粒子方向单位向量
	
	//中心放射状属性成员
	Vector2D radius = Vector2D(0, 0);			///<内外半径 [内径，外径]
	Vector2D scoop = Vector2D(0, 360);			///放射范围  [起始范围，结束范围]

	//线性放射状属性成员	只能在确定的角度生成线,粒子跟线成90度
	float length = 1.f;				///<线长度
	float angle = 0.f;				///<相对于线的角度

	///生产器 
	///@brief 相当于初始化粒子结构数据
	void produce();
public:
	Particle() :sizeRange(Vector2D(0.75, 1.25)) { lasttime = steady_clock::now(); };
	/**
	 * 加载粒子资源.
	 * 
	 * \param [in] name 动画资源名
	 */
	void Load(std::string name);

	/**
	 * 设置粒子发射最大初速度.
	 * 
	 * \param [in] speed 最大速度
	 */
	void SetMaxSpeed(float speed) { this->maxSpeed = speed; }

	/**
	 * 设置粒子发射最小初速度.
	 * 
	 * \param [in] speed 最小速度
	 */
	void SetMinSpeed(float speed) { this->minSpeed = speed; }

	/**
	 * @brief 设置大小范围（倍率系数）
	 * @param[in] lower			    最小倍率
	 * @param[in] upper				最大倍率
	 **/
	void SetSizeRange(float lower, float upper) { sizeRange = { lower,upper }; }

	virtual void Update(float deltaTime)override;
	virtual void Render()override;

	///设置粒子容量
	void SetCapacity(int capacity) { this->capacity = capacity; }
	/*void SetSpeed(float speed) { this->speed = speed; }*/

	///设置粒子重力
	void SetGravity(int gravity) { this->gravity = gravity; }

	///设置单个粒子生命周期 
	void SetLifetime(int lifetime) { this->lifetime = lifetime; }

	///设置粒子是否持续生产 
	void SetIsLoop(bool isloop) { this->isLoop = isloop; }

	//设置粒子生产间隔
	void SetInterval(float Interval) { this->Interval = Interval; }

	//设置粒子渐变消失时间 
	void SetFadingtime(float fadingtime) { this->fadingtime = fadingtime; }

	//设置粒子模式
	void SetPattern(ParticlePattern pattern) { this->pattern = pattern; }

	/**
	 * @brief 设置中心放射状参数
	 * @param[in] radius			放射圆环的内外径
	 * @param[in] scoop				放射扇形的角度范围
	 **/
	void SetCenter(Vector2D radius, Vector2D scoop) { this->radius = radius; this->scoop = scoop; }

	/**
	 * @brief 设置线性放射状参数
	 * @remark 默认初始坐标系为向下的y  外部认知为顺时针
	 * @param[in] length			放射线段长度
	 * @param[in] angle	            线段与水平夹角 根据坐标系方向
	 **/
	void SetLine(float length, float angle) { this->length = length; this->angle = angle; }
};