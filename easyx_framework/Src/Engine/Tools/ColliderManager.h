/*****************************************************************//**
 * \file   ColliderManager.h
 * \brief  碰撞管理器
 * 
 * \author zhn
 *********************************************************************/

#pragma once
#include"unordered_map"
#include"unordered_set"
#include<string>

/**
 * 碰撞种类
 * 需要用户自己设置种类.
 */
enum class CollisionType :uint8_t
{
	Default,			///<默认种类
	sunshine,
	plateform,
	Zomble,
	plant,
	Bullet
};

		/*思路*/
/*用哈希映射实现碰撞物体的存储变少和碰撞检测变少 说白了就是进行碰撞体种类名的映射			*/
/*存在单向或者双向的关系*/
/**
 * 碰撞管理器
 * 
 *  用哈希映射实现碰撞物体的存储变少和碰撞检测变少 说白了就是进行碰撞体种类名的映射.
 *  存在单向或者双向的关系.
 * 一个正整数可以唯一表示为两个随机整数的平方和
 */
class ColliderManager
{
	//1.
	/*std::unordered_map<std::string, std::unordered_set<std::string>>collidermap;	*/		//实现映射，某个物体种类存储的是跟它可以发生碰撞检测的物体种类

	//一个正整数可以唯一表示为两个随机整数的平方和
	std::unordered_set<int> collisionmap;		///<实现相当于哈希映射 节省消耗
public:

	//物体种类映射，默认没有和没有之间进行判定
	ColliderManager() {
		//1.
		/*collidermap.insert({ "Default",std::unordered_set<std::string>{"Default" } });*/
		collisionmap.insert(int(CollisionType::Default)*int(CollisionType::Default)+ int(CollisionType::Default) * int(CollisionType::Default));
		collisionmap.insert(int(CollisionType::plateform) * int(CollisionType::plateform) + int(CollisionType::sunshine) * int(CollisionType::sunshine));
		collisionmap.insert(int(CollisionType::sunshine) * int(CollisionType::sunshine) + int(CollisionType::sunshine) * int(CollisionType::sunshine));
		AddMapping(CollisionType::Bullet, CollisionType::Zomble);
		Initialize();
}

	///初始化映射关系
	void Initialize();

	///关键字之间是否存在映射
	/*bool ExistMapping(const std::string& key1,const std::string& key2);*/
	bool ExistMapping(CollisionType type1,CollisionType type2);

	///查找与关键字有碰撞关系的种类
	/*std::unordered_set<std::string>* FindMapping(const std::string& key1);*/
	std::unordered_set<int> FindMapping(CollisionType type1);

	/*void AddMapping(const std::string& key1, const std::string& key2);*/
	void AddMapping(CollisionType type1, CollisionType type2);
};