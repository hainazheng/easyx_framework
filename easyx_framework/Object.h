/*****************************************************************//**
 * \file   Object.h
 * \brief  ��Ϸ�������
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include<unordered_set>
#include"CoreMinimal.h"
#include"Log.h"
#include"SceneComponent.h"

class Component;
class SceneComponent;

/**
 * ��Ϸ�������.
 */
class Object :public base
{
	std::unordered_set<Object*> children;									///<��������
	Object* parent = nullptr;									///<ʵ������֮��İ�  ���� ��һ���������Ӱ���������ʵ��һ���ƶ�ʵ����Ч

	std::unordered_set<Component*> components;						///<�����洢ָ����ʵ�ǵ�ַ��ָ������ν�治���ڣ�ֻҪȡ���ĵ�ַ��һ���µ�ָ�����
	std::unordered_set<Component*>::iterator components_iter;			///<��������ı�������ɾ

protected:
	SceneComponent* const root = new SceneComponent;							///<��������������賡������  �մ�������һ��ͬʱ���������ĳ������,������ı�ָ��

public:
	///ֻ��һ�����������(���г�������)������Ҫ���������Բ���Ҫ��������
	Object() { root->SetOwner(this); }				
	///���ɾ���Ŀ���Ȩ�ڶ�����
	~Object()																
	{
		for (auto& com : components)
		{

			/*std::cout << components.size()<<std::endl;*/
			delete com;
		}
		delete root;
	}

	///��������ÿ�����
	virtual void Update(float deltaTime)override				
	{

		components_iter = components.begin();
		while (components_iter != components.end())
		{
			(*components_iter)->Update(deltaTime);
			if (components_iter == components.end())break;			//��ֹ��ɾ�����ĵ������ı䶯���� ֻҪָ��β�����һ��������ѭ��
			components_iter++;

		}
	}

	///�����麯�����Ѳ���д�ڹ���ȴ��Ҫ����ʵ�ֵ�д����
	virtual void BeginPlay();

	virtual void EndPlay();

	/**
	 * ������������ע�����.
	 * @remark �������û�����
	 * \param [in]pCom ���ָ�루������༫�����ࣩ
	 */
	void RegisterComponent(Component* pCom)
	{
		components.insert(pCom);
	}

	/**
	 * ���������������ɾ����Ӧ���.
	 * @remark �������û�����
	 * \param [in] pCom ���ָ�루������༫�����ࣩ
	 */
	void UnregisterComponent(Component* pCom)
	{
		auto it = components.find(pCom);		//��������Ѱ�����
		components_iter = components.erase(it);			//����ɾ��֮��ĺ�̽ڵ�
	}

	/**
	 * �����������ĸ���ƥ������.
	 * ģ�庯��<������༫������>
	 * \return �ҵ��ķ�����Ӧ���ָ�� û�о�nullptr
	 */
	template<class T>
	T* GetComponentByClass()
	{
		for (auto& com : components)
		{
			if (T* pCom = dynamic_cast<T*>(com))return pCom;			//ֻҪ������ȡ���Ķ�����ת������Ҫ�����ͣ��Ͱ�������
		}
		return nullptr;
	}

	/**
	 * ����������󶨵����󣬲�ע�ᵽ��������.
	 * ģ�庯��<������༫������>
	 * �൱�ڹ�����װ�õĴ������ 
	 * ������ͳһ����������������
	 * \return  �ɹ�������Ӧ���ָ�� ʧ�ܾ�nullptr
	 */
	template<class T>
	T* ConstructComponent()
	{
		T* pCom = new T;
		if (pCom && static_cast<Component*>(pCom))		//�þ�̬ת������Ϊ����ת��Ϊ���࣬���ܸ���ת��������
		{
			pCom->SetOwner(this);
			RegisterComponent(pCom);
			pCom->BeginPlay();
			return pCom;
		}
		delete pCom;
		Log::instance()->printf(Log::LEVEL_WAR, __FILE__, __LINE__,
			"�������ʧ��");
		return nullptr;
	}

	/**
	 * ������������ ʵ�������.
	 * 
	 * \param [in] par ������ָ��
	 */
	void AttachTo(Object* par);
	/**
	 * ����������� ��������.
	 * ����������
	 * \param [in] par ��ָ��
	 */
	void DetachForm(Object* par);

	/**
	 * ���ó�����������������ĸ��������������Ϊ�Լ��ĳ�������.
	 * 
	 * \param [in] newroot �µĸ����
	 */
	void SetRootComponent(SceneComponent* newroot);

	void Destroy();			//��������ȱ���ʵ�ֱ������������ɾ��������

	///��ȡ�����������
	Vector2D GetLocalPosition() const {
		if (root != nullptr) {/* std::cout << 1 << std::endl;*/return root->GetLocalPosition(); }
		return Vector2D(0, 0);
	}		
	///��ȡ������ԽǶ�
	float GetLocalRotation() const { if (root != nullptr)return root->GetLocalRotation(); return 0; }			
	///��ȡ����������Ŵ�С
	Vector2D GetLocalScale() const { if (root != nullptr)return root->GetLocalScale(); return Vector2D(0, 0); }			
	Vector2D GetWorldPosition() const;		//��ȡ�����������
	float GetWorldRotation() const;			//��ȡ������ԽǶ�
	Vector2D GetWorldScale() const;			//��ȡ����������Ŵ�С

	///���������������
	void SetLocalPosition(const Vector2D& pos) { if (root != nullptr)root->SetLocalPosition(pos); }			
	///����������ԽǶ�
	void SetLocalRotation(float rot) { if (root != nullptr)root->SetLocalRotation(rot); }					
	///��������������Ŵ�С
	void SetLocalScale(const Vector2D& scale) { if (root != nullptr)root->SetLocalScale(scale); }			
	
	///����һ������
	void AddPosition(const Vector2D& pos) { root->AddPosition(pos); }			
	///����һ���Ƕ�
	void AddRotation(float rot) { root->AddRotation(rot); }						

	virtual void DrawDebugPosition()const;
};