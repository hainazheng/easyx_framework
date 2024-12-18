/*****************************************************************//**
 * \file   World.h
 * \brief  ��Ϸ�����࣬���л���������֯�߼�
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"
#include"Object.h"
#include <mutex>

///ʵ����Ⱦͼ������º���
/// �����Զ���˳��
/// @see ����layer��С��������
struct LayerSort
{
	bool operator()(const class LayerInterface* a, const class LayerInterface* b)const;
};

///ʵ����ײͼ������º���
/// �����Զ���˳��
/// @see ����layer��С��������
struct ColliderSort
{
	bool operator()(const class  Collider* a, const class Collider* b)const;
};

///��Ϸ����
///@remark �洢ȫ�����ж��󣬲������ⲿ�̳��޸�
class World final			
{
	//��Ԫ ����Ȩ��
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


	std::mutex updateMutex;		///<������ ��ֹĳһ��ʱ�̺�ʱ���࣬������һ��ִ�г�ͻ ��һֱ�ȴ���ֹ˳���δ֪

	///��Դ��
	///@remark �ڳ�ʼ������
	class ResourceManager* resourcePool = nullptr;		//�ڳ�ʼ������

	///��ײӳ���ϵ������
	/// @remark �ڳ�ʼ������ 
	class ColliderManager* collisionManager = nullptr;	//�ڳ�ʼ������

	/// ��ͼ������
	///  @remark �ڳ�ʼ������
	class LevelManager* levelManager = nullptr;

	/// ȫ������������ 
	class AudioManager* audiomanager = nullptr;

	/// ȫ����־��
	class Log* log = nullptr;

	/// ȫ����������
	class Configurator* m_cfg = nullptr;

	///ȫ�ּ�ʱ�� 
	/// @brief ��¼��ܿ�ʼ�˶��
	class Timer* OverallClock = nullptr;

	///֡���ʱ�� ��֮֡���ʱ��
	/// @see ����һ֡��
	float deltaTime = 0;

	//��������UI,��ʱ������
	std::unordered_set<Object*> GameObjects;						///<�洢������Ϸ����
	std::vector<Object*> GameObjects_to_add;						///<����������������� �����γ���ͬһ��������Ϊ����createobject�������ͼ���
	std::unordered_set<Object*> GameObjects_to_delete;				///<���ö����ɾ��������������������Ϊ�˷�ֹ����ɾ��֮��������ջ���©���˶���
	std::unordered_set<class UserInterface*> GameUIs;				///<�洢������ϷUI
	std::unordered_set<class UserInterface*> GameUIs_to_delete;		///<ɾ����ϷUI������
	std::unordered_set<class Timer*>GameTimers;						///<��ʱ������

	//��ײ�߼���gamecollidersÿ����ײ������������жϲ����ĸ�����(һ��������п��ܻ��ж������)����ÿ�������ڵ��������ж�

	//��Ⱦ����ײ��������
	std::set<class LayerInterface*, LayerSort>GameRenderers;				///< �洢����֧����Ⱦ�������
	std::unordered_set<class Collider*> GameColliders;						///<�ײ��ϣ���������ظ� �洢������ײ�������
	std::unordered_set<class Collider*>GameColliders_to_clear;				///< ��ɾ����ײ���������
	std::set<class Collider*, ColliderSort>ColliderZones[RangeNumX][RangeNumY];				///<�洢ÿ������洢����ײ�� ����Ϊ100*100�����񣬶���100�����������ܱ������Ĳ��ֵ���������
	std::set<class LayerInterface*, LayerSort>UiDetectZones[RangeNumX][RangeNumY];			///<�洢ÿ������洢��ui�������н���



	///֡���ļ�ʱ��
	class Timer* FPSclock = nullptr;

	//��Ϸ��������
	Level* CurrentLevel = nullptr;					///<��ǰ�ؿ�
	class Controller* mainController = nullptr;
	GameInstance* gameInstance = nullptr;			///<��Ϸʵ�� ��Ҫ���ȥ�ĵ��� ����д��ȥ

	///��Ϸ��������� Ҳ������Ļ
	class Camera* mainCamera = nullptr;				

	//�����߼�����
	///�߼�����
	void Update(float deltaTime);

	///�߾��ȼ���
	void Update_();				//�߾��ȼ���

	///ת���ؿ�
	void TransForm();

	///��Ⱦ
	void Render();

	///�����������룩
	void Input();

	///Debugģʽ
	void Debug();

	///����colliderzone ��ײ����
	void ProcessColliderZones();

	///�������
	void ClearWorld();
};


//�ⲿ���� ȫ��Ψһ��ȫ����������������Ϸ��Ҫ�Ķ���
extern World mainWorld;


