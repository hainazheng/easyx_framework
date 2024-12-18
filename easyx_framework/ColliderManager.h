/*****************************************************************//**
 * \file   ColliderManager.h
 * \brief  ��ײ������
 * 
 * \author zhn
 *********************************************************************/

#pragma once
#include"unordered_map"
#include"unordered_set"
#include<string>

/**
 * ��ײ����
 * ��Ҫ�û��Լ���������.
 */
enum class CollisionType :uint8_t
{
	Default,			///<Ĭ������
	sunshine,
	plateform,
	Zomble,
	plant,
	Bullet
};

		/*˼·*/
/*�ù�ϣӳ��ʵ����ײ����Ĵ洢���ٺ���ײ������ ˵���˾��ǽ�����ײ����������ӳ��			*/
/*���ڵ������˫��Ĺ�ϵ*/
/**
 * ��ײ������
 * 
 *  �ù�ϣӳ��ʵ����ײ����Ĵ洢���ٺ���ײ������ ˵���˾��ǽ�����ײ����������ӳ��.
 *  ���ڵ������˫��Ĺ�ϵ.
 * һ������������Ψһ��ʾΪ�������������ƽ����
 */
class ColliderManager
{
	//1.
	/*std::unordered_map<std::string, std::unordered_set<std::string>>collidermap;	*/		//ʵ��ӳ�䣬ĳ����������洢���Ǹ������Է�����ײ������������

	//һ������������Ψһ��ʾΪ�������������ƽ����
	std::unordered_set<int> collisionmap;		///<ʵ���൱�ڹ�ϣӳ�� ��ʡ����
public:

	//��������ӳ�䣬Ĭ��û�к�û��֮������ж�
	ColliderManager() {
		//1.
		/*collidermap.insert({ "Default",std::unordered_set<std::string>{"Default" } });*/
		collisionmap.insert(int(CollisionType::Default)*int(CollisionType::Default)+ int(CollisionType::Default) * int(CollisionType::Default));
		collisionmap.insert(int(CollisionType::plateform) * int(CollisionType::plateform) + int(CollisionType::sunshine) * int(CollisionType::sunshine));
		collisionmap.insert(int(CollisionType::sunshine) * int(CollisionType::sunshine) + int(CollisionType::sunshine) * int(CollisionType::sunshine));
		AddMapping(CollisionType::Bullet, CollisionType::Zomble);
		Initialize();
}

	///��ʼ��ӳ���ϵ
	void Initialize();

	///�ؼ���֮���Ƿ����ӳ��
	/*bool ExistMapping(const std::string& key1,const std::string& key2);*/
	bool ExistMapping(CollisionType type1,CollisionType type2);

	///������ؼ�������ײ��ϵ������
	/*std::unordered_set<std::string>* FindMapping(const std::string& key1);*/
	std::unordered_set<int> FindMapping(CollisionType type1);

	/*void AddMapping(const std::string& key1, const std::string& key2);*/
	void AddMapping(CollisionType type1, CollisionType type2);
};