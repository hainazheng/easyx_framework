#include "Particle.h"
#include"ResourceManager.h"
#include"Math.h"
#include"camera.h"
#include"World.h"

void Particle::produce()
{
	ParticleInfo temp;
	temp.alpha = alpha;
	temp.index = Math::RandInt(0, number-1);			//���ȡ����ĳһ��֡��
	temp.size = Math::RandReal(sizeRange.x, sizeRange.y);		//���ȡ��С
	if (pattern == ParticlePattern::Center)
	{
		double radian = Math::RandReal(scoop.x, scoop.y) * PI / 180.f;		//�������
		unitVector = Vector2D(cos(radian), sin(radian));				//��λ����

		//��������⾶ ����������������ĳ��ԭ�㣨���������ߣ���ƫ�����������������λ�ã�
		if (radius != Vector2D(0, 0)) { temp.offset = unitVector * min( radius.x,radius.y) + abs(radius.y - radius.x) * unitVector * Math::RandPerc(); }
		temp.velocity = Math::RandReal(minSpeed, maxSpeed) * unitVector;
	}
	else if (pattern == ParticlePattern::Line)
	{
		//Ĭ�ϳ�ʼ����ϵΪ���µ�y  �ⲿ��֪Ϊ˳ʱ��
		double radian = -angle * PI / 180.f;
		unitVector = Vector2D(cos(radian), sin(radian));
		Vector2D lineVector=Vector2D::RotateVector(90, unitVector);
		//�г���
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

	//��������
	if (particle.size() < capacity)
	{
		//һ��������
		if (Interval == 0) { produce(); lasttime = steady_clock::now(); }
		else if (duration<float>(steady_clock::now() - lasttime).count() > Interval)
		{
			//�ﵽʱ����
			produce(); lasttime = steady_clock::now();
		}

		//һ�����������Ӵﵽ���������Ҳ�ѭ������
		if(!isLoop&& particle.size() >= capacity)
		{
			capacity = -1;		//�´ξͲ����д���
		}
	}

	//�������� һ���ﵽ�������ھ�����
	if (particle.size())
	{
		if (Interval == 0)
		{
			//һ���������ľ�ֻҪ����ʱ���������һ�����ӵ�ʱ����Ǵ�����������
			if (duration<float>(steady_clock::now() - (particle.begin())->lasttime).count() > lifetime)
			{
				particle.clear();

			}
			else if (fadingtime && duration<float>(steady_clock::now() - (particle.begin())->lasttime).count() > lifetime - fadingtime)
			{
				//͸�����𽥱�С
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

	//�����Ӳ����������Խ��й���
	for (auto& each : particle)
	{
		if (gravity)each.velocity.y += gravity * deltaTime;
		each.offset += each.velocity * deltaTime;
				if (fadingtime && duration<float>(steady_clock::now() - each.lasttime).count() > lifetime - fadingtime)
				{
					//͸�����𽥱�С
					 const_cast<ParticleInfo&>(each).alpha -= alpha * deltaTime / fadingtime;
				}
	}
}

void Particle::Render()
{
	
	if (!bIsEnabled || !images)return;
	
		
		Vector2D center = (GetWorldPosition() - mainWorld.mainCamera->GetCameraPosition());

		HDC dstDC = GetImageHDC();				//����ƽ���ͼƬ���
		
		int w = images[0]->getwidth();			//IMAGE�����Դ���ȡ����Ŀ��
		int h = images[0]->getheight();		//IMAGE�����Դ���ȡ����ĸ߶�

		float dw = int(w * GetWorldScale().x * 20.f / mainWorld.mainCamera->springArmLength_virtual);
		float dh = int(h * GetWorldScale().y * 20.f / mainWorld.mainCamera->springArmLength_virtual);
		for (auto& each : particle)
		{
			
			BLENDFUNCTION bf = { AC_SRC_OVER,0,each.alpha,AC_SRC_ALPHA };				//����͸���ȣ���Աȫ��BYTE����
			Vector2D  pos= center+each.offset;
			pos = pos * (20.f / mainWorld.mainCamera->springArmLength_virtual) + Vector2D(getwidth() / 2, getheight() / 2) - Vector2D(dw / 2, dh / 2);
			HDC srcDC = GetImageHDC(images[each.index]);			//Ҫ���ݵ�ͼƬ���
			AlphaBlend(dstDC, (int)pos.x , (int)pos.y , dw, dh, srcDC, 0, 0, w, h, bf);			//�൱��putimage ճ������Ӧ�Ĺ���ƽ��
		}
	
}
