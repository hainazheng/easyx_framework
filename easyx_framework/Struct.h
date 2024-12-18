/*****************************************************************//**
 * \file   Struct.h
 * \brief  常用结构体定义
 * 
 * \author zhn
 *********************************************************************/


#pragma once
#include<iostream>
#include<string>

constexpr double PI = 3.1415926;

///二维向量
/// @brief 精度较高的二维向量
struct Vector2D
{
	float x;		
	float y;
	Vector2D() :x(0), y(0) {}
	Vector2D(float a, float b) :x(a), y(b) {}
	
	///自己的向量长度
	float size()
	{
		return sqrtf(x * x + y * y);
	}

	///单位向量
	Vector2D unit()
	{
		return Vector2D(x / size(), y / size());
	}

	///获取两个坐标之间的距离(标量)
	/// @param 向量1
	/// @param 向量2
	/// @return 距离标量
	static float Distance(const Vector2D& a, const Vector2D& b)
	{
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}

	///将向量转化为字符串
	/// @return 格式(x,y)
	std::string To_String() const
	{
		return "(" + std::to_string(int(x)) + "," + std::to_string(int(y)) + ")";
	}

	///向量点积
	/// @return v1·v2
	static float DotProduct(const Vector2D& v1, const Vector2D& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
	//向量点积
	//static float DotProduct(Vector2D v1, Vector2D v2)
	//{
	//	return v1.x * v2.x + v1.y * v2.y;
	//}

	///向量叉积
	/// @return v1×v2
	static Vector2D CrossProduct(const Vector2D& v1, const Vector2D& v2)
	{
		return { v1.y * v2.x - v1.x * v2.y, v1.x * v2.y - v1.y * v2.x };
	}

	///旋转某一向量，返回旋转后的向量（单位是角度）
	/// @brief 注意坐标系方向 默认顺时针
	static Vector2D RotateVector(float angle, Vector2D& another)
	{
		float radian = angle * PI / 180.0;			//转化为弧度
		float fcos = (float)cos(radian);
		float fsin = (float)sin(radian);

		return Vector2D(another.x * fcos - another.y * fsin, another.x * fsin + another.y * fcos);
	}

	///向量投影 
	/// u投影到v上
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

////重载前缀-
//static Vector2D& operator-(Vector2D& another)
//{
//	another.x = -another.x;
//	another.y = -another.y;
//	return another;
//}
///重载前缀-,不是可引用左值的版本
static Vector2D operator-(Vector2D another)
{
	another.x = -another.x;
	another.y = -another.y;
	return another;
}

///重载<<显示
static std::ostream& operator<<(std::ostream& out, const Vector2D& another)
{
	std::cout << '(' << another.x << ',' << another.y << ')';
	return out;
}

///重载[float]*[Vector2D]
static Vector2D operator*(float scale, Vector2D& me)
{
	return Vector2D(me.x * scale, me.y * scale);
}

///整数数据对
/// @brief 存储整数
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
 * 材质结合方式.
 */
enum class CombinePattern :unsigned char { 
	Min,	///<取每个系数最小的对象的值
	Mid,	///<取每个系数平均值
	Max		///<取每个系数最大的对象的值
};

/**
 * 物理材质类.
 */
struct PhysicsMaterial
{
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

	
	float friction = 0.2f;			///<摩擦系数
	
	float bounciness;				///<弹性系数
	PhysicsMaterial() :friction(0.2f), bounciness(0) {}
	PhysicsMaterial(float friction, float bounciness)
		:friction(friction), bounciness(bounciness) {}

	///物理材质结合效果 中间混合模式就是系数取中间值 最小就是取两个系数中的最小值
	/// @return 最后算出的系数的材质类
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
 * 场景属性类.
 */
struct Transform
{

	Vector2D position;		///<位置坐标
	float rotation;		///<角度
	Vector2D scale;		///<缩放尺寸

	Transform() :position(0, 0), rotation(0), scale(1, 1) {}
	Transform(const Vector2D& pos, float rot, const Vector2D& sca) :
		position(pos), rotation(rot), scale(sca) {}

	///特征
	/// @return 坐标原点、角度0、原尺寸
	Transform Indentity()const
	{
		return Transform(Vector2D(0, 0), (float)0, Vector2D(1, 1));
	}
};

/**
*	矩形框
 * @brief 盒型坐标.
 */
struct Rect
{
	float left						///<左边
		, top						///<上面的边
		, right						///<右边的边
		, bottom;					///<下面的边

	Rect(float _left = 0, float _top = 0, float _right = 0, float _bottom = 0)
		: left(_left), top(_top), right(_right), bottom(_bottom) {}

	/// 获取矩形的中心点
	Vector2D GetCenter() const { return { (left + right) / 2, (bottom + top) / 2 }; }

	///获取矩形大小
	Vector2D GetSize() const { return { right - left,bottom -top  }; }

	/// 检查两个矩形是否相交
	bool Intersects(const Rect& other) const
	{
		return left < other.right && right > other.left && bottom > other.top && top < other.bottom;
	}
};

/// 检查两个矩形是否相交
static bool Intersects(const Rect& me,const Rect& other)
{
	return me.left < other.right && me.right > other.left && me.bottom > other.top && me.top < other.bottom;
}