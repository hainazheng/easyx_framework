/*****************************************************************//**
 * \file   AniEdge.h
 * \brief  ��������
 * @brief �����붯����״̬ת������һЩ�����ṹ����
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include<string>
#include"CoreMinimal.h"
#include"Log.h"


/**
 * ת�������Ƚ�.
 */
enum class TransitionComparison : uint8
{
	Equal,					///<���
	NotEqual,				///<����
	Greater,				///<����
	Less,					///<С��
	GreaterEqual,			///<���ڵ���
	LessEqual				///<С�ڵ���
};


/**
 * ����ת�������������ռ�.
 */
namespace TransitionCondition {
	
	/**
	 * ��������.
	 */
	struct Integer {
		std::string paramName;				///<��������
		int value;							///<ֵ
		TransitionComparison comparison;	///<ת�������Ƚ�
	};
	
	/**
	 * ����������.
	 */
	struct Float {
		std::string paramName;				///<��������
		float value;						///<ֵ
		TransitionComparison comparison;	///<ת�������Ƚ�
	};

	/**
	 * ����������
	 * @remarkҪ��ֵ��ͬTRUE��ͬFALSE�ͻ�������.
	 */
	struct Bool {
		std::string paramName;				///<��������
		bool value;							///<ֵ
	};

	/**
	 * ����������.
	 * @remark ֻ��TRUE�Ż�����
	 */
	struct Trigger {
		std::string paramName;				///<��������
	};

	/**
	 * �Ƚ�ģʽ.
	 * @remark AND��������������
	 * @remark ORֻҪ����һ��
	 */
	enum ComparisonMode
	{
		AND				///<��������������
		, OR			///<ֻҪ����һ��
	};

	/**
	 * ��ȡ��ͬ�����ȽϽ��.
	 * 
	 * \param [in] a �Ƚ�ֵ1 һ��������ʵ�ʵ�ֵ
	 * \param [in] b �Ƚ�ֵ2 һ�������Ƕ����ֵvalue
	 * \param [in] comp  ת�������Ƚ�
	 * \return �ȽϽ�� true����������
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
 * ������(�����).
 * һ���߿����ж������
 * @reamrk ��Ҫɾȥ��ֻҪ�ڴ洢������͵ĳ����ڵ���ɾȥ����
 */
template<class F>
class CondiEdge final
{
	friend class Animator;
	friend class Animation;
	friend class FSM;
	friend class FSMItems;

	F* start;			///<�����ڵ� ��������� ����
	F* end;				///<����ڵ�

	std::vector<TransitionCondition::Integer> integerConditions;			///<������������
	std::vector<TransitionCondition::Float> floatConditions;				///<��������������
	std::vector<TransitionCondition::Bool> boolConditions;					///<��������������
	std::vector<TransitionCondition::Trigger> triggerConditions;			///<������������

	TransitionCondition::ComparisonMode comparisonMode;						///<ת������ģʽ

public:
	CondiEdge(F* start=nullptr, F* end=nullptr, TransitionCondition::ComparisonMode mode = TransitionCondition::AND)
	{
		Init(start, end, mode);
	}

	/**
	 * @brief ���ɳ�ʼ��(��֧������һ��û��ָ���)
	 * @param[in] start			    �����ڵ�
	 * @param[in] end	            ����ڵ�
	 * @param[in] mode              ���ɱȽ�ģʽ��AND��Ҫ��������������ORֻ����������������
	 **/
	void Init(F* start, F* end, TransitionCondition::ComparisonMode mode = TransitionCondition::AND) {
		if (static_cast<F*>(start) && static_cast<F*>(end)) {
			//Debug û��ת��start��end���ǿ�ָ��
			if (start ==nullptr|| end==nullptr) {
			Log::instance()->printf(Log::LEVEL_ERR, __FILE__, __LINE__,
				"��ʼ����ʧ��"); 
			return; 
			}

			this->start = start; this->end = end; comparisonMode = mode; this->start->nexts.push_back(this); 
		}
			
	};



	/**
	 * ������ι�������.
	 *
	 * \param [in] condition �µ���������
	 */
	void AddCondition(const TransitionCondition::Integer& condition)
	{
		integerConditions.push_back(condition);
	}

	/**
	 * ��Ӹ����������.
	 *
	 * \param [in] condition �µĸ�������
	 */
	void AddCondition(const TransitionCondition::Float& condition)
	{
		floatConditions.push_back(condition);
	}

	/**
	 * ��Ӳ����͹�������.
	 *
	 * \param [in] condition �µĲ�������
	 */
	void AddCondition(const TransitionCondition::Bool& condition)
	{
		boolConditions.push_back(condition);
	}

	/**
	 * ��Ӵ�����������.
	 *
	 * \param [in] condition �µĴ�������
	 */
	void AddCondition(const TransitionCondition::Trigger& condition)
	{
		triggerConditions.push_back(condition);
	}

	/**
	 * �Ƿ��޹�������.
	 *
	 * \return 1����û�й�������
	 */
	bool IsUnconditional()const
	{
		return integerConditions.empty() && floatConditions.empty() && boolConditions.empty() && triggerConditions.empty();
	}
};

/**
 * ״̬��������.
 * @brief ����ʶ����������
 */
enum class ParamType : uint8
{
	Integer			///<����
	, Float			///<������
	, Bool			///<������
	, Trigger		///<������
};

///����ĳ�������ಢ����������
#define DECLARE_EDGE_CONDITION_BY_TYPE(name,type)typedef class CondiEdge<type> name;
