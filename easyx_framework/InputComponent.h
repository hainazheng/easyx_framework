/*****************************************************************//**
 * \file   InputComponent.h
 * \brief  ���뽻�����
 * @brief ��������������߼�����
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"
#include<functional>
#include<map>
#include"easyx.h"
#include"Component.h"

///�����ź�����
enum  class InputType:uint8
{
	Pressed,		///<����
	Released,		///<�ͷ�
	Holding,		///<һֱ���� �ύ�� Ƶ�ʸ�����
	DoubleClick		///<˫��
};

///Ԥ�������ֵ
enum KeyCode
{
	VK_LButton = 1,
	VK_RButton,
	VK_MButton = 4,
	VK_wheeledUp,
	VK_wheeledDown,
	VK_A=0x41,
	VK_B,
	VK_C,
	VK_D,
	VK_E,
	VK_F,
	VK_G,
	VK_H,
	VK_I,
	VK_J,
	VK_K,
	VK_L,
	VK_M,
	VK_N,
	VK_O,
	VK_P,
	VK_Q,
	VK_R,
	VK_S,
	VK_T,
	VK_U,
	VK_V,
	VK_W,
	VK_X,
	VK_Y,
	VK_Z,
	//VK_MOUSEMOVE
};

/**
 * ����ӳ����Ҫ����Ϣ.
 */
struct KeyBindInfo
{
	std::function<void()> fun;					//����ָ��ʵ�ָ���ֵ��
	InputType TYPE = InputType::Pressed;
	bool isPressed = false;
	float lastTime = 0;
};

/**
 * ���뽻�����
 * @brief ���������ֵ����ӳ��.
 */
class InputComponent :public Component
{
private:
	std::map<std::string, int> mappings;					///<ʵ�ּ�ֵ�����ֵ�ӳ�� [����,����ֵ]
	std::map<std::string, KeyBindInfo>actionCallbacks;		///<ʵ�����ֺͺ������ܰ� [���֣�������Ҫ����Ϣ]
	

	static Vector2D mousePos;							///<����ڻ�ͼ��������
	static bool bActive;								///�Ƿ����ý���

	ExMessage msg;
public:
	/**
	 * ���ü�ֵ�����ֵ�ӳ��.
	 * 
	 * \param [in] mappingName ����
	 * \param [in] value ����ֵ
	 */
	void SetMapping(std::string mappingName, int value);

	virtual void  BeginPlay() {};

	/**
	 * �󶨺����ͼ�ֵ.
	 * 
	 * \param [in] ActionName  ����
	 * \param [in] type �����ź�����
	 * \param [in] obj  �����
	 * \param [in] func  ��Ա����
	 */
	template<typename T>
	void BindAction(std::string ActionName, InputType type, T* obj, void(T::* func)())
	{
		//��Ҫ��mappings�ҵĵ���Ӧ��ӳ������
		if (mappings.find(ActionName) != mappings.end())
		{
			actionCallbacks.insert({ ActionName,{std::bind(func,obj),type,false} });
		}
	}

	/**
	 * �󶨺����ͼ�ֵ,
	 * @remark û��ӳ��mapping��ֱ�ӽ��м�λӳ��.
	 * \param [in] ActionName  ����
	 * @param [in] value �����ֵ
	 * \param [in] type �����ź�����
	 * \param [in] obj  �����
	 * \param [in] func  ��Ա����
	 */
	template<typename T>
	void BindAction(std::string ActionName, int value, InputType type, T* obj, void(T::* func)())
	{
		//��Ҫ��mappings�ҵĵ���Ӧ��ӳ������,û�ҵ��ʹ���
		if (mappings.find(ActionName) != mappings.end())
		{
			actionCallbacks.insert({ ActionName,{std::bind(func,obj),type,false} });
		}
		else
		{
			SetMapping(ActionName, value);
			actionCallbacks.insert({ ActionName,{std::bind(func,obj),type,false} });
		}
	}

	/**
	 * �󶨰����¼�ӳ��.
	 * @remark ���Բ���Ҫ����� ֱ�Ӻ���ӳ��
	 * \param [in] ActionName  ����
	 * \param [in] type �����ź�����
	 * \param [in] func  lambda����
	 */
	void BindAction(std::string ActionName, InputType type, std::function<void()>func)
	{
		if (mappings.find(ActionName) != mappings.end())
			actionCallbacks.insert({ ActionName, { func,type,false } });
	}

	/**
	 * action.
	 * @remark��˲ʱ����������Ƶ����֡��Ӱ�죩
	 */
	void PeekInfo();

	/**
	 * �ᴦ��.
	 * �̶�Ƶ��ִ��
	 */
	void PeekInfo_();

	/**
	 * ��ȡ����ڻ�ͼ��������.
	 * 
	 */
	void MouseTick();

	virtual void Update(float deltaTime)override;

	/**
	 * ��ȡ�����Ļ���� ����ڻ���λ��.
	 * 
	 * \return �����(0,0)��
	 */
	static Vector2D GetMousePosition();

	/**
	 * �������Ƿ���.
	 * 
	 * \return �Ƿ������
	 */
	static bool IsMouseButtonClick();

	/**
	 * �����������.
	 * 
	 * \param [in] enable �Ƿ񼤻��������
	 */
	static void EnableInput(bool enable);
};