/*****************************************************************//**
 * \file   GameStatics.h
 * \brief  ȫ�־�̬�ⶨ�壬�ṩ������Ϸ�����߼�����Ľӿڣ���������ʲ�������Ϸ����
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"
#include<vector>
#include"UserInterface.h"
#include"Object.h"
#include"World.h"
/**
 * ȫ�־�̬��.
 */
class GameStatics final			
{
public:
	///��ȡ��Ϸʵ��
	static GameInstance* GetGameInstance();

	///��ȡ��ҿ�����
	/// @remark ����û�оͻᴴ���µĿ�����
	static Controller* GetController();


	///��������ʵ������,ʵ�ָ������������������
	/// @remark ��������������ֻ�����������
	/// @see Object
	template<class T>
	static T* CreateObject(Vector2D pos = Vector2D(0, 0)	///<[in]λ������
	);

	///��world����������������Ҫ�ҵ��༰������Ž���������
	template<class T>
	static std::vector<T*> FindObjectsofClass();

	///����UI����
	///@remark ����������UserInterfaceֻ�����������
	/// @see UserInterface
	template<class T>
	static T* CreateUI();


	///���ص�ͼ
	static void OpenLevel(std::string levelName			///<[in]�ؿ�������
	);

	///�����
	static void PlayCameraShake(
		int intensity			///<[in]��ǿ��
		, int decay = 20		///<[in]��˥��ϵ�� Ĭ��Ϊ20
	);

	///��ȡ��Ϸ����ʱ��
	static double GetTimeSeconds();

	///��ͣ��Ϸ����
	static void Pause(float delay		///<[in]����
	);

	///������ЧƬ��
	static void PlaySound2D(
		std::string name		///<[in]��Ƶ����
	);
};

template<class T>
inline T*  GameStatics::CreateObject(Vector2D pos )
{
	T* pObj = new T;
	if (pObj && static_cast<Object*>(pObj))		//�þ�̬ת������Ϊ����ת��Ϊ���࣬���ܸ���ת��������
	{
		mainWorld.GameObjects_to_add.push_back(pObj);
		pObj->SetLocalPosition(pos);
		return pObj;
	}
	return nullptr;
}

template<class T>
inline std::vector<T*> GameStatics::FindObjectsofClass()
{
	std::vector<T*> result;
	//Ԥ�����ڴ� ����ֹ����ɾ��ʱ��Ƶ���������ڴ� ռ���ڴ���Դ
	result.reserve(mainWorld.GameObjects.size());
	for (auto& obj : mainWorld.GameObjects)
	{
		if (T* pObj = static_cast<T*>(obj))result.push_back(pObj);
	}
	return result;
}

template<class T>
inline T* GameStatics::CreateUI()
{
	T* pUI = new T;
	if (pUI && static_cast<UserInterface*>(pUI))
	{
		mainWorld.GameUIs.insert(pUI);
		return pUI;
	}
	delete pUI;
	return nullptr;
}