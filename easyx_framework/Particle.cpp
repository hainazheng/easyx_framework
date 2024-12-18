#include "Particle.h"
#include"ResourceManager.h"
#include"Math.h"
#include"camera.h"
#include"World.h"

void Particle::produce()
{
	ParticleInfo temp;
	temp.alpha = alpha;
	temp.index = Math::RandInt(0, number-1);			//随机取动画某一个帧数
	temp.size = Math::RandReal(sizeRange.x, sizeRange.y);		//随机取大小
	if (pattern == ParticlePattern::Center)
	{
		double radian = Math::RandReal(scoop.x, scoop.y) * PI / 180.f;		//随机弧度
		unitVector = Vector2D(cos(radian), sin(radian));				//单位向量

		//如果有内外径 计算这个粒子相对于某个原点（粒子生产者）的偏移量（大多数是坐标位置）
		if (radius != Vector2D(0, 0)) { temp.offset = unitVector * min( radius.x,radius.y) + abs(radius.y - radius.x) * unitVector * Math::RandPerc(); }
		temp.velocity = Math::RandReal(minSpeed, maxSpeed) * unitVector;
	}
	else if (pattern == ParticlePattern::Line)
	{
		//默认初始坐标系为向下的y  外部认知为顺时针
		double radian = -angle * PI / 180.f;
		unitVector = Vector2D(cos(radian), sin(radian));
		Vector2D lineVector=Vector2D::RotateVector(90, unitVector);
		//有长度
		if (length) { temp.offset = Math::RandPerc()*lineVector*length-length/2.f*lineVector; }
		temp.velocity = Math::RandReal(minSpeed, maxSpeed) * unitVector;
	}
	particle.push_back(temp);
}

void Particle::Load(std::string name)
{
	AnimationResource ani = mainWorld.resourcePool->AnimationFetch(name);
	
	number = ani.num;
	images = ani.images;
}

void Particle::Update(float deltaTime)
{
	SceneComponent::Update(deltaTime);

	if (!bIsEnabled)return;

	/*if(Interval!=0) Interval = Math::Clamp(Interval, DELTA_TIME, 100);*/

	if (Interval != 0) Interval = Math::Clamp(Interval, deltaTime, 100);

	//生产粒子
	if (particle.size() < capacity)
	{
		//一次性生产
		if (Interval == 0) { produce(); lasttime = steady_clock::now(); }
		else if (duration<float>(steady_clock::now() - lasttime).count() > Interval)
		{
			//达到时间间隔
			produce(); lasttime = steady_clock::now();
		}

		//一旦创建的粒子达到容量，并且不循环创建
		if(!isLoop&& particle.size() >= capacity)
		{
			capacity = -1;		//下次就不进行创建
		}
	}

	//销毁粒子 一旦达到生命周期就销毁
	if (particle.size())
	{
		if (Interval == 0)
		{
			//一次性生产的就只要现在时间跟创建第一个粒子的时间差是大于生命周期
			if (duration<float>(steady_clock::now() - (particle.begin())->lasttime).count() > lifetime)
			{
				particle.clear();

			}
			else if (fadingtime && duration<float>(steady_clock::now() - (particle.begin())->lasttime).count() > lifetime - fadingtime)
			{
				//透明度逐渐变小
				for (auto& each : particle) { const_cast<ParticleInfo&>(each).alpha -= alpha * deltaTime / fadingtime; }
			}
		}
		else
		{

			if (duration<float>(steady_clock::now() - particle.begin()->lasttime).count() > lifetime)
			{
				particle.pop_front();

			}

		}
	}

	//对粒子参数物理属性进行管理
	for (auto& each : particle)
	{
		if (gravity)each.velocity.y += gravity * deltaTime;
		each.offset += each.velocity * deltaTime;
				if (fadingtime && duration<float>(steady_clock::now() - each.lasttime).count() > lifetime - fadingtime)
				{
					//透明度逐渐变小
					 const_cast<ParticleInfo&>(each).alpha -= alpha * deltaTime / fadingtime;
				}
	}
}

void Particle::Render()
{
	
	if (!bIsEnabled || !images)return;
	
		
		Vector2D center = (GetWorldPosition() - mainWorld.mainCamera->GetCameraPosition());

		HDC dstDC = GetImageHDC();				//工作平面的图片句柄
		
		int w = images[0]->getwidth();			//IMAGE类型自带获取对象的宽度
		int h = images[0]->getheight();		//IMAGE类型自带获取对象的高度

		float dw = int(w * GetWorldScale().x * 20.f / mainWorld.mainCamera->springArmLength_virtual);
		float dh = int(h * GetWorldScale().y * 20.f / mainWorld.mainCamera->springArmLength_virtual);
		for (auto& each : particle)
		{
			
			BLENDFUNCTION bf = { AC_SRC_OVER,0,each.alpha,AC_SRC_ALPHA };				//设置透明度，成员全是BYTE类型
			Vector2D  pos= center+each.offset;
			pos = pos * (20.f / mainWorld.mainCamera->springArmLength_virtual) + Vector2D(getwidth() / 2, getheight() / 2) - Vector2D(dw / 2, dh / 2);
			HDC srcDC = GetImageHDC(images[each.index]);			//要传递的图片句柄
			AlphaBlend(dstDC, (int)pos.x , (int)pos.y , dw, dh, srcDC, 0, 0, w, h, bf);			//相当于putimage 粘贴到相应的工作平面
		}
	
}
