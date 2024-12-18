/*****************************************************************//**
 * \file   Particle.h
 * \brief  2D�������
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

///���ӷ������ʽ
enum class ParticlePattern:uint8
{
	Center,			///<���ķ���״
	Line,			///<���Է���״ ֻ����ȷ���ĽǶ�������,���Ӹ��߳�90��
};


///��¼������Ϣ��
struct ParticleInfo
{
	Vector2D offset;		///<���ڼ�¼������������Ӵ���������λ��
	int index=0;			///<��ǰ֡���� �������������ͼƬ
	Vector2D velocity;		///<�ٶ�
	time_point<steady_clock> lasttime;		///<���Ӵ���ʱ��
	float alpha = 255.f;			///<͸���� ʵ��������͸��Ч��
	float size = 1.f;				///<��С
	ParticleInfo(){ lasttime = steady_clock::now(); }
};





/**
 * ������� 
 * @remark �����͹�����������.
 * @remark ����Ⱦ������ ������Ҫ����ͼƬ��֡�� ��ʵ��������䲻ͬ֡����ͼƬ
 */
class Particle :public SceneComponent, public LayerInterface, public ImageInterface
{
	std::deque<ParticleInfo> particle;		///<�������� ���ڹ�������������ÿһ������
	int capacity = 0;			///<��������

	//float speed = 1.f;			//�����ٶȿ����ı���
	float maxSpeed = 1.f;			///<���������ٶ�
	float minSpeed = 1.f;			///<������С���ٶ�
	float gravity = 9.8f;			///<�������ٶ�
	float lifetime = 1.f;			///<�������ӵ���������
	bool isLoop = true;				///<�Ƿ�ѭ������
	float Interval=0.1f;			///<�������
	///��������ʧ��ʱ�� 
	///@brief ��Ҫ��ʵ�ֹ���Ч��
	float fadingtime = 0;			


	IMAGE** images=nullptr;					///<����֡����
	int number = 0;							///<����������֡��
	time_point<steady_clock> lasttime;		///<��¼������ʱ���
	Vector2D sizeRange;						///<��С��Χ������ϵ����

	ParticlePattern pattern = ParticlePattern::Center;		////<���ӷ���״ģʽ
	Vector2D unitVector = Vector2D(0, 1);		///<���ӷ���λ����
	
	//���ķ���״���Գ�Ա
	Vector2D radius = Vector2D(0, 0);			///<����뾶 [�ھ����⾶]
	Vector2D scoop = Vector2D(0, 360);			///���䷶Χ  [��ʼ��Χ��������Χ]

	//���Է���״���Գ�Ա	ֻ����ȷ���ĽǶ�������,���Ӹ��߳�90��
	float length = 1.f;				///<�߳���
	float angle = 0.f;				///<������ߵĽǶ�

	///������ 
	///@brief �൱�ڳ�ʼ�����ӽṹ����
	void produce();
public:
	Particle() :sizeRange(Vector2D(0.75, 1.25)) { lasttime = steady_clock::now(); };
	/**
	 * ����������Դ.
	 * 
	 * \param [in] name ������Դ��
	 */
	void Load(std::string name);

	/**
	 * �������ӷ��������ٶ�.
	 * 
	 * \param [in] speed ����ٶ�
	 */
	void SetMaxSpeed(float speed) { this->maxSpeed = speed; }

	/**
	 * �������ӷ�����С���ٶ�.
	 * 
	 * \param [in] speed ��С�ٶ�
	 */
	void SetMinSpeed(float speed) { this->minSpeed = speed; }

	/**
	 * @brief ���ô�С��Χ������ϵ����
	 * @param[in] lower			    ��С����
	 * @param[in] upper				�����
	 **/
	void SetSizeRange(float lower, float upper) { sizeRange = { lower,upper }; }

	virtual void Update(float deltaTime)override;
	virtual void Render()override;

	///������������
	void SetCapacity(int capacity) { this->capacity = capacity; }
	/*void SetSpeed(float speed) { this->speed = speed; }*/

	///������������
	void SetGravity(int gravity) { this->gravity = gravity; }

	///���õ��������������� 
	void SetLifetime(int lifetime) { this->lifetime = lifetime; }

	///���������Ƿ�������� 
	void SetIsLoop(bool isloop) { this->isLoop = isloop; }

	//���������������
	void SetInterval(float Interval) { this->Interval = Interval; }

	//�������ӽ�����ʧʱ�� 
	void SetFadingtime(float fadingtime) { this->fadingtime = fadingtime; }

	//��������ģʽ
	void SetPattern(ParticlePattern pattern) { this->pattern = pattern; }

	/**
	 * @brief �������ķ���״����
	 * @param[in] radius			����Բ�������⾶
	 * @param[in] scoop				�������εĽǶȷ�Χ
	 **/
	void SetCenter(Vector2D radius, Vector2D scoop) { this->radius = radius; this->scoop = scoop; }

	/**
	 * @brief �������Է���״����
	 * @remark Ĭ�ϳ�ʼ����ϵΪ���µ�y  �ⲿ��֪Ϊ˳ʱ��
	 * @param[in] length			�����߶γ���
	 * @param[in] angle	            �߶���ˮƽ�н� ��������ϵ����
	 **/
	void SetLine(float length, float angle) { this->length = length; this->angle = angle; }
};