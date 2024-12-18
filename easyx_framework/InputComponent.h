/*****************************************************************//**
 * \file   InputComponent.h
 * \brief  输入交互组件
 * @brief 玩家鼠标键盘输入逻辑处理
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"CoreMinimal.h"
#include<functional>
#include<map>
#include"easyx.h"
#include"Component.h"

///按键信号类型
enum  class InputType:uint8
{
	Pressed,		///<按下
	Released,		///<释放
	Holding,		///<一直按着 轴交互 频率更高受
	DoubleClick		///<双击
};

///预设虚拟键值
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
 * 按键映射需要的信息.
 */
struct KeyBindInfo
{
	std::function<void()> fun;					//函数指针实现跟键值绑定
	InputType TYPE = InputType::Pressed;
	bool isPressed = false;
	float lastTime = 0;
};

/**
 * 输入交互组件
 * @brief 输入输出键值控制映射.
 */
class InputComponent :public Component
{
private:
	std::map<std::string, int> mappings;					///<实现键值和名字的映射 [名字,虚拟值]
	std::map<std::string, KeyBindInfo>actionCallbacks;		///<实现名字和函数功能绑定 [名字，按键需要的信息]
	

	static Vector2D mousePos;							///<鼠标在绘图窗口坐标
	static bool bActive;								///是否启用交互

	ExMessage msg;
public:
	/**
	 * 设置键值和名字的映射.
	 * 
	 * \param [in] mappingName 键名
	 * \param [in] value 虚拟值
	 */
	void SetMapping(std::string mappingName, int value);

	virtual void  BeginPlay() {};

	/**
	 * 绑定函数和键值.
	 * 
	 * \param [in] ActionName  键名
	 * \param [in] type 按键信号类型
	 * \param [in] obj  类对象
	 * \param [in] func  成员函数
	 */
	template<typename T>
	void BindAction(std::string ActionName, InputType type, T* obj, void(T::* func)())
	{
		//需要在mappings找的到对应的映射名称
		if (mappings.find(ActionName) != mappings.end())
		{
			actionCallbacks.insert({ ActionName,{std::bind(func,obj),type,false} });
		}
	}

	/**
	 * 绑定函数和键值,
	 * @remark 没有映射mapping就直接进行键位映射.
	 * \param [in] ActionName  键名
	 * @param [in] value 虚拟键值
	 * \param [in] type 按键信号类型
	 * \param [in] obj  类对象
	 * \param [in] func  成员函数
	 */
	template<typename T>
	void BindAction(std::string ActionName, int value, InputType type, T* obj, void(T::* func)())
	{
		//需要在mappings找的到对应的映射名称,没找到就创建
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
	 * 绑定按键事件映射.
	 * @remark 可以不需要类对象 直接函数映射
	 * \param [in] ActionName  键名
	 * \param [in] type 按键信号类型
	 * \param [in] func  lambda函数
	 */
	void BindAction(std::string ActionName, InputType type, std::function<void()>func)
	{
		if (mappings.find(ActionName) != mappings.end())
			actionCallbacks.insert({ ActionName, { func,type,false } });
	}

	/**
	 * action.
	 * @remark（瞬时触发，触发频率受帧率影响）
	 */
	void PeekInfo();

	/**
	 * 轴处理.
	 * 固定频率执行
	 */
	void PeekInfo_();

	/**
	 * 获取鼠标在绘图窗口坐标.
	 * 
	 */
	void MouseTick();

	virtual void Update(float deltaTime)override;

	/**
	 * 获取鼠标屏幕坐标 相对于画布位置.
	 * 
	 * \return 相对于(0,0)点
	 */
	static Vector2D GetMousePosition();

	/**
	 * 鼠标左键是否按下.
	 * 
	 * \return 是否按下鼠标
	 */
	static bool IsMouseButtonClick();

	/**
	 * 激活输入输出.
	 * 
	 * \param [in] enable 是否激活输入输出
	 */
	static void EnableInput(bool enable);
};