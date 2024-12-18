/*****************************************************************//**
 * \file   Struct.h
 * \brief  ���ýṹ�嶨��
 * 
 * \author zhn
 *********************************************************************/


#pragma once
#include<iostream>
#include<string>

constexpr double PI = 3.1415926;

///��ά����
/// @brief ���ȽϸߵĶ�ά����
struct Vector2D
{
	float x;		
	float y;
	Vector2D() :x(0), y(0) {}
	Vector2D(float a, float b) :x(a), y(b) {}
	
	///�Լ�����������
	float size()
	{
		return sqrtf(x * x + y * y);
	}

	///��λ����
	Vector2D unit()
	{
		return Vector2D(x / size(), y / size());
	}

	///��ȡ��������֮��ľ���(����)
	/// @param ����1
	/// @param ����2
	/// @return �������
	static float Distance(const Vector2D& a, const Vector2D& b)
	{
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}

	///������ת��Ϊ�ַ���
	/// @return ��ʽ(x,y)
	std::string To_String() const
	{
		return "(" + std::to_string(int(x)) + "," + std::to_string(int(y)) + ")";
	}

	///�������
	/// @return v1��v2
	static float DotProduct(const Vector2D& v1, const Vector2D& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
	//�������
	//static float DotProduct(Vector2D v1, Vector2D v2)
	//{
	//	return v1.x * v2.x + v1.y * v2.y;
	//}

	///�������
	/// @return v1��v2
	static Vector2D CrossProduct(const Vector2D& v1, const Vector2D& v2)
	{
		return { v1.y * v2.x - v1.x * v2.y, v1.x * v2.y - v1.y * v2.x };
	}

	///��תĳһ������������ת�����������λ�ǽǶȣ�
	/// @brief ע������ϵ���� Ĭ��˳ʱ��
	static Vector2D RotateVector(float angle, Vector2D& another)
	{
		float radian = angle * PI / 180.0;			//ת��Ϊ����
		float fcos = (float)cos(radian);
		float fsin = (float)sin(radian);

		return Vector2D(another.x * fcos - another.y * fsin, another.x * fsin + another.y * fcos);
	}

	///����ͶӰ 
	/// uͶӰ��v��
	static Vector2D ProjectVector(Vector2D& u, Vector2D& v)
	{
		float normproU = DotProduct(v, u) / v.size();
		return v.unit() * normproU;
	}

	Vector2D operator+(const Vector2D& another)
	{
		return Vector2D(x + another.x, y + another.y);
	}
	Vector2D operator-(const Vector2D& another)
	{
		return Vector2D(x - another.x, y - another.y);
	}
	Vector2D& operator+=(const Vector2D& another)
	{
		x = x + another.x;
		y = y + another.y;
		return *this;
	}
	Vector2D& operator-=(const Vector2D& another)
	{
		x = x - another.x;
		y = y - another.y;
		return *this;
	}
	Vector2D operator*(float scale)const
	{

		return Vector2D(x * scale, y * scale);
	}
	Vector2D& operator*=(float scale)
	{
		x = x * scale;
		y = y * scale;
		return *this;
	}
	Vector2D operator*(const Vector2D& another)const
	{

		return Vector2D(x * another.x, y * another.y);
	}
	bool operator==(const Vector2D& another)
	{
		if (x == another.x && y == another.y)
			return true;
		else
		{
			return false;
		}
	}

	Vector2D operator/(const Vector2D& another)
	{
		return Vector2D(x / another.x, y / another.y);
	}
	Vector2D operator/(float scale)const
	{

		return Vector2D(x / scale, y / scale);
	}
	bool operator==(const Vector2D& another)const
	{
		if (x == another.x && y == another.y)
			return true;
		else
		{
			return false;
		}
	}
	bool operator!=(const Vector2D& another)const
	{
		if (x != another.x || y != another.y)
			return true;
		else
		{
			return false;
		}
	}
};

////����ǰ׺-
//static Vector2D& operator-(Vector2D& another)
//{
//	another.x = -another.x;
//	another.y = -another.y;
//	return another;
//}
///����ǰ׺-,���ǿ�������ֵ�İ汾
static Vector2D operator-(Vector2D another)
{
	another.x = -another.x;
	another.y = -another.y;
	return another;
}

///����<<��ʾ
static std::ostream& operator<<(std::ostream& out, const Vector2D& another)
{
	std::cout << '(' << another.x << ',' << another.y << ')';
	return out;
}

///����[float]*[Vector2D]
static Vector2D operator*(float scale, Vector2D& me)
{
	return Vector2D(me.x * scale, me.y * scale);
}

///�������ݶ�
/// @brief �洢����
struct Pair
{
	int x;
	int y;
	Pair() :x(0), y(0) {}
	Pair(int x, int y) :x(x), y(y) {}
	Pair(Vector2D vec) :x(int(vec.x)), y(int(vec.y)) {}
	bool operator==(const Pair& another) const
	{
		return x == another.x && y == another.y;
	}
	bool operator!=(const Pair& another) const
	{
		return x != another.x || y != another.y;
	}
};

/**
 * ���ʽ�Ϸ�ʽ.
 */
enum class CombinePattern :unsigned char { 
	Min,	///<ȡÿ��ϵ����С�Ķ����ֵ
	Mid,	///<ȡÿ��ϵ��ƽ��ֵ
	Max		///<ȡÿ��ϵ�����Ķ����ֵ
};

/**
 * ���������.
 */
struct PhysicsMaterial
{
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

	
	float friction = 0.2f;			///<Ħ��ϵ��
	
	float bounciness;				///<����ϵ��
	PhysicsMaterial() :friction(0.2f), bounciness(0) {}
	PhysicsMaterial(float friction, float bounciness)
		:friction(friction), bounciness(bounciness) {}

	///������ʽ��Ч�� �м���ģʽ����ϵ��ȡ�м�ֵ ��С����ȡ����ϵ���е���Сֵ
	/// @return ��������ϵ���Ĳ�����
	static PhysicsMaterial Combine(const PhysicsMaterial& m1, const PhysicsMaterial& m2, CombinePattern pattern = CombinePattern::Mid)
	{
		if (pattern == CombinePattern::Mid)
		{
			return PhysicsMaterial((m1.friction + m2.friction) * 0.5f, (m1.bounciness + m2.bounciness) * 0.5f);
		}
		else
		{
			return pattern == CombinePattern::Min ? PhysicsMaterial(min(m1.friction, m2.friction), min(m1.bounciness, m2.bounciness))
				: PhysicsMaterial(max(m1.friction, m2.friction), max(m1.bounciness, m2.bounciness));
		}
	}
};

/**
 * ����������.
 */
struct Transform
{

	Vector2D position;		///<λ������
	float rotation;		///<�Ƕ�
	Vector2D scale;		///<���ųߴ�

	Transform() :position(0, 0), rotation(0), scale(1, 1) {}
	Transform(const Vector2D& pos, float rot, const Vector2D& sca) :
		position(pos), rotation(rot), scale(sca) {}

	///����
	/// @return ����ԭ�㡢�Ƕ�0��ԭ�ߴ�
	Transform Indentity()const
	{
		return Transform(Vector2D(0, 0), (float)0, Vector2D(1, 1));
	}
};

/**
*	���ο�
 * @brief ��������.
 */
struct Rect
{
	float left						///<���
		, top						///<����ı�
		, right						///<�ұߵı�
		, bottom;					///<����ı�

	Rect(float _left = 0, float _top = 0, float _right = 0, float _bottom = 0)
		: left(_left), top(_top), right(_right), bottom(_bottom) {}

	/// ��ȡ���ε����ĵ�
	Vector2D GetCenter() const { return { (left + right) / 2, (bottom + top) / 2 }; }

	///��ȡ���δ�С
	Vector2D GetSize() const { return { right - left,bottom -top  }; }

	/// ������������Ƿ��ཻ
	bool Intersects(const Rect& other) const
	{
		return left < other.right && right > other.left && bottom > other.top && top < other.bottom;
	}
};

/// ������������Ƿ��ཻ
static bool Intersects(const Rect& me,const Rect& other)
{
	return me.left < other.right && me.right > other.left && me.bottom > other.top && me.top < other.bottom;
}