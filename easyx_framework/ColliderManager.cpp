#include "ColliderManager.h"

/**
 * ����ӳ���ϵ��д������.
 * 
 */
void ColliderManager::Initialize()
{
	AddMapping(CollisionType::plant, CollisionType::Zomble);
	/*AddMapping(CollisionType::Bullet, CollisionType::Zomble);*/
	
}
/**
 * 
 * ��CollisionType�м�����Ӧ������
 * \param ��ײ����1
 * \param ��ײ����1
 * \return 
 */
bool ColliderManager::ExistMapping(CollisionType type1, CollisionType type2)
{
	int value = (int)type1 * (int)type1 + (int)type2 * (int)type2;
	auto it = collisionmap.find(value);
	if (it != collisionmap.end())
	{
		return true;
	}
	return false;
}



//bool ColliderManager::ExistMapping(const std::string& key1, const std::string& key2)
//{
//	auto it = collidermap.find(key1);
//	if (it != collidermap.end())
//	{
//		 return it->second.find(key2)!=it->second.end();
//	}
//	return false;
//}




//std::unordered_set<std::string>* ColliderManager::FindMapping(const std::string& key1)
//{
//	auto it = collidermap.find(key1);
//	if (it != collidermap.end())
//	{
//		return &(it->second);
//	}
//	return nullptr;
//}

/**
 * 
 * 
 * \param ��ײ����
 * \return int�͵�����
 */
std::unordered_set<int> ColliderManager::FindMapping(CollisionType type1)
{
	std::unordered_set<int> collidertype;
	for (auto it = collisionmap.begin(); it!=collisionmap.end();it++)
	{
		if (sqrt((*it) - (int)type1 * (int)type1) == static_cast<int>((*it) - (int)type1 * (int)type1))
		{
			int value =static_cast<int>((*it) - (int)type1 * (int)type1);
			collidertype.insert(value);
		}
	}
	return collidertype;
}




//���߶��������Է������������
//void ColliderManager::AddMapping(const std::string& key1, const std::string& key2)
//{
//	if(collidermap.find(key1)==collidermap.end())
//	{
//		collidermap.insert({ key1,std::unordered_set<std::string>()});
//	}
//	if (collidermap.find(key2) == collidermap.end())
//	{
//		collidermap.insert({ key2,std::unordered_set<std::string>() });
//	}
//	collidermap[key1].insert(key2);
//	collidermap[key2].insert(key1);
//}


/**
 * �����ײ�����ӳ��.
 * 
 * \param ��ײ����1
 * \param ��ײ����2
 */
void ColliderManager::AddMapping(CollisionType type1, CollisionType type2)
{
	int value = (int)type1 * (int)type1 + (int)type2 * (int)type2;
	
	if (collisionmap.find(value) != collisionmap.end())
	{
		return;
	}
	collisionmap.insert(value);
}