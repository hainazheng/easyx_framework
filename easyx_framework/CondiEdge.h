/*****************************************************************//**
 * \file   AniEdge.h
 * \brief  动画过渡
 * @brief 包含与动画及状态转换条件一些基础结构和类
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include<string>
#include"CoreMinimal.h"
#include"Log.h"


/**
 * 转换条件比较.
 */
enum class TransitionComparison : uint8
{
	Equal,					///<相等
	NotEqual,				///<不等
	Greater,				///<大于
	Less,					///<小于
	GreaterEqual,			///<大于等于
	LessEqual				///<小于等于
};


/**
 * 动画转换条件的命名空间.
 */
namespace TransitionCondition {
	
	/**
	 * 整形条件.
	 */
	struct Integer {
		std::string paramName;				///<参数名字
		int value;							///<值
		TransitionComparison comparison;	///<转换条件比较
	};
	
	/**
	 * 浮点型条件.
	 */
	struct Float {
		std::string paramName;				///<参数名字
		float value;						///<值
		TransitionComparison comparison;	///<转换条件比较
	};

	/**
	 * 布尔型条件
	 * @remark要跟值是同TRUE或同FALSE就会是满足.
	 */
	struct Bool {
		std::string paramName;				///<参数名字
		bool value;							///<值
	};

	/**
	 * 触发型条件.
	 * @remark 只有TRUE才会满足
	 */
	struct Trigger {
		std::string paramName;				///<参数名字
	};

	/**
	 * 比较模式.
	 * @remark AND所有条件都满足
	 * @remark OR只要满足一个
	 */
	enum ComparisonMode
	{
		AND				///<所有条件都满足
		, OR			///<只要满足一个
	};

	/**
	 * 获取相同参数比较结果.
	 * 
	 * \param [in] a 比较值1 一般是我们实际的值
	 * \param [in] b 比较值2 一般是我们定义的值value
	 * \param [in] comp  转换条件比较
	 * \return 比较结果 true是满足条件
	 */
	template<typename T>
	bool GetComparisonResult(T a, T b, TransitionComparison comp)
	{
		switch (comp) {
		case TransitionComparison::Equal:return a == b;
		case TransitionComparison::Greater:return a > b;
		case TransitionComparison::GreaterEqual:return a >= b;
		case TransitionComparison::Less:return a < b;
		case TransitionComparison::LessEqual:return a <= b;
		case TransitionComparison::NotEqual:return a != b;
		}
		return false;
	}
}

class Animator;
class Animation;

/**
 * 过渡类(有向边).
 * 一个边可以有多个条件
 * @reamrk 想要删去边只要在存储这个类型的出发节点中删去就行
 */
template<class F>
class CondiEdge final
{
	friend class Animator;
	friend class Animation;
	friend class FSM;
	friend class FSMItems;

	F* start;			///<出发节点 由他储存边 单向
	F* end;				///<到达节点

	std::vector<TransitionCondition::Integer> integerConditions;			///<整形条件容器
	std::vector<TransitionCondition::Float> floatConditions;				///<浮点型条件容器
	std::vector<TransitionCondition::Bool> boolConditions;					///<布尔型条件容器
	std::vector<TransitionCondition::Trigger> triggerConditions;			///<触发条件容器

	TransitionCondition::ComparisonMode comparisonMode;						///<转换条件模式

public:
	CondiEdge(F* start=nullptr, F* end=nullptr, TransitionCondition::ComparisonMode mode = TransitionCondition::AND)
	{
		Init(start, end, mode);
	}

	/**
	 * @brief 过渡初始化(不支持任意一边没有指向空)
	 * @param[in] start			    出发节点
	 * @param[in] end	            到达节点
	 * @param[in] mode              过渡比较模式（AND需要满足所有条件，OR只需满足任意条件）
	 **/
	void Init(F* start, F* end, TransitionCondition::ComparisonMode mode = TransitionCondition::AND) {
		if (static_cast<F*>(start) && static_cast<F*>(end)) {
			//Debug 没有转化start和end都是空指针
			if (start ==nullptr|| end==nullptr) {
			Log::instance()->printf(Log::LEVEL_ERR, __FILE__, __LINE__,
				"初始化边失败"); 
			return; 
			}

			this->start = start; this->end = end; comparisonMode = mode; this->start->nexts.push_back(this); 
		}
			
	};



	/**
	 * 添加整形过渡条件.
	 *
	 * \param [in] condition 新的整形条件
	 */
	void AddCondition(const TransitionCondition::Integer& condition)
	{
		integerConditions.push_back(condition);
	}

	/**
	 * 添加浮点过渡条件.
	 *
	 * \param [in] condition 新的浮点条件
	 */
	void AddCondition(const TransitionCondition::Float& condition)
	{
		floatConditions.push_back(condition);
	}

	/**
	 * 添加布尔型过渡条件.
	 *
	 * \param [in] condition 新的布尔条件
	 */
	void AddCondition(const TransitionCondition::Bool& condition)
	{
		boolConditions.push_back(condition);
	}

	/**
	 * 添加触发过渡条件.
	 *
	 * \param [in] condition 新的触发条件
	 */
	void AddCondition(const TransitionCondition::Trigger& condition)
	{
		triggerConditions.push_back(condition);
	}

	/**
	 * 是否无过渡条件.
	 *
	 * \return 1代表没有过渡条件
	 */
	bool IsUnconditional()const
	{
		return integerConditions.empty() && floatConditions.empty() && boolConditions.empty() && triggerConditions.empty();
	}
};

/**
 * 状态参数类型.
 * @brief 用于识别条件类型
 */
enum class ParamType : uint8
{
	Integer			///<整形
	, Float			///<浮点型
	, Bool			///<布尔型
	, Trigger		///<触发型
};

///创建某个过渡类并命名个别名
#define DECLARE_EDGE_CONDITION_BY_TYPE(name,type)typedef class CondiEdge<type> name;
