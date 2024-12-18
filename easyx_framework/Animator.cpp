#include "Animator.h"
#include"World.h"
#include"ResourceManager.h"
#include"SpriteRenderer.h"
#include"Sprite.h"

//���ض�����Դ
void Animation::Load(std::string name, Vector2D delta)
{
	AnimationResource ani = mainWorld.resourcePool->AnimationFetch(name);
	
	num = ani.num;
	images = ani.images;
	offset = delta;
}

void Animation::Tick_()
{
	//�������ѭ����ᾲֹ�����һ֡(ǰ�᲻����̫�涯��)
	if (index == num - 1 && !bLooping  && !bMontage)
	{
		clock.Stop(); return;
	}
	UpdateIndex();
	if (notification.find(index) != notification.end())notification[index].Execute();

	//��Ϊ��̫�涯�����򲥷�һ����������� 
	//�Ѿ�ִ����һ���� ��ʱ����һ��ִ�е����±��Ѿ���0��
	if (index == 0 && bMontage)
	{
		bMontage = false;
		if (OnMontageExit.IsBinded())OnMontageExit.Execute();
		else
		{
			for (auto edge : nexts)
			{
				if (animController->CheckConditions(edge))
				{
					animController->SetNode(edge->end);
					break;
				}
			}
		}
	}

	//���޹����������򲥷�һ�����������һ���ڵ�
	for (auto edge : nexts)
	{
		if ((edge->IsUnconditional() && index == 0))
		{
			animController->SetNode(edge->end);
			break;
		}
	}
}

Animator::~Animator()
{
	if (rendererAttached)rendererAttached->animatorAttached = nullptr;
	for (auto& obj : animations)
	{
		obj.second.animController = nullptr;
	}
}

/**
 * ��ȡ�������Ⱦ���.
 * 
 */
void Animator::BeginPlay()
{
	if (rendererAttached)return;

	rendererAttached = pOwner->GetComponentByClass<SpriteRenderer>();
	if (rendererAttached)rendererAttached->animatorAttached = this;
}

///��ȡ��Ⱦ�� ������Ⱦ ���ҽ�����������
void Animator::Update(float deltaTime)
{
	//if (!rendererAttached)
	//{
	//	rendererAttached=pOwner->GetComponentByClass<SpriteRenderer>();		//��ȡ��Ⱦ��
	//	
	//}
	//else if (aninode)
	//{
	//	rendererAttached->sprite = aninode->images[aninode->index];
	//	rendererAttached->spriteinfo.offset = aninode->offset;
	//	
	//	/*std::cout << rendererAttached << std::endl;*/
	//}
	Component::Update(deltaTime);
	if (!rendererAttached || !aninode)return;

	IMAGE* sprite = aninode->images[aninode->index];
	if (currentSprite != sprite)
	{
		if (currentSprite && !bIsEnabled)return;

		currentSprite = sprite;
		
		rendererAttached->sprite = sprite;
		rendererAttached->spriteinfo.offset = aninode->offset;

		///ʵ������λ�ö��ڶ����Ǵ����Ǹ�λ�� һ�������Ͻǵ�
		rendererAttached->spriteinfo.EndLoc = { sprite->getwidth(), sprite->getheight() };


		rendererAttached->DealImage();
	}

	//����������
	if (aninode->bMontage)return;
	for (auto edge : aninode->nexts)
	{
		if (CheckConditions(edge))
		{
			SetNode(edge->end);
			break;
		}
	}
}


/**
 * ���붯����Դ����������.
 * 
 * \param [in] name ��������
 * \param [in] ani ������Դ
 */
void Animator::insert(std::string name, Animation& ani)
{
	//������Դ����Ϊ��
	if(ani.images)
	animations.insert({ name,ani });
	ani.animController = this;

}

/**
 * ���õ�ǰ���ŵĽڵ�.
 * @remark (������ֱ��ʹ��)
 * \param [in] name ��������
 */
void Animator::SetNode(std::string name)
{
	//ֹͣ��ǰ�������� ��ֹ��Դռ�� debug2024.9.20��������ж�aninode�ǿվͻ����
	if (aninode) { aninode->clock.Stop(); aninode->OnAnimExit.Execute(); }
	
	//���ýڵ�
	aninode = &(animations.find(name)->second);
	aninode->index = 0;
	//��ֹ�ڵ��ϴ��Ѿ�������ֹͣʱ��
	aninode->clock.Continue();
	aninode->OnAnimEnter.Execute();
	
}

/**
 * ���õ�ǰ���ŵĽڵ�.
 * @remark (������ֱ��ʹ��)
 * \param [in]node �����ӿ���
 */
void Animator::SetNode(Animation* node)
{
	if (aninode)
	{
		aninode->clock.Stop(); aninode->OnAnimExit.Execute();
	}

	aninode = node;
	aninode->index = 0;
	aninode->clock.Continue();
	aninode->OnAnimEnter.Execute();
}

void Animator::SetupAttachment(SpriteRenderer* renderer)
{
	rendererAttached = renderer;
	renderer->animatorAttached = this;
}

///�Ƿ񲥷Ŷ���
void Animator::SetCalled(bool called)
{
	called ? aninode->clock.Continue() : aninode->clock.Stop();
}

void Animator::PlayMontage(std::string nodeName)
{
	if (aninode == &(animations.find(nodeName)->second))//����ǰ���ڲ�����ͬ��̫�涯����������Ϊ��һ֡
	{
		aninode->index = 0; return;
	}
	lastnode = aninode;
	SetNode(nodeName);
	aninode->bMontage = true;

	if (aninode->nexts.empty())
		aninode->OnMontageExit.Bind([this]()//����̫��ڵ����(�޺�̽ڵ㣩���򲥷���Ϻ�Ĭ�ϻָ�֮ǰ�Ķ����ڵ� 
			{
				aninode->clock.Stop(); aninode->OnAnimExit.Execute();
				aninode = lastnode;
				aninode->clock.Continue();
				aninode->OnAnimEnter.Execute();
			});


}


void Animator::AddParamater(std::string paramName, ParamType type)
{
	if (type == ParamType::Integer)
	{
		parametersInteger.insert({ paramName,int() });
	}
	else if (type == ParamType::Float)
	{
		parametersFloat.insert({ paramName,float() });
	}
	else if (type == ParamType::Bool)
	{
		parametersBool.insert({ paramName,bool() });
	}
	else
	{
		parametersTrigger.insert({ paramName,false });
	}
}

void Animator::SetInteger(std::string paramName, int value)
{
	if (parametersInteger.find(paramName) != parametersInteger.end())
	{
		parametersInteger[paramName] = value;
	}
}

void Animator::SetFloat(std::string paramName, float value)
{
	if (parametersFloat.find(paramName) != parametersFloat.end())
	{
		parametersFloat[paramName] = value;
	}
}

void Animator::SetBool(std::string paramName, bool value)
{
	if (parametersBool.find(paramName) != parametersBool.end())
	{
		parametersBool[paramName] = value;
	}
}

void Animator::SetTrigger(std::string paramName)
{
	if (parametersTrigger.find(paramName) != parametersTrigger.end())
	{
		parametersTrigger[paramName] = true;
	}
}

bool Animator::CheckConditions(AnimEdge* edge)
{
	bool result = false;
	for (const auto& condition : edge->integerConditions) {

		if (parametersInteger.find(condition.paramName) == parametersInteger.end())continue;
		result = TransitionCondition::GetComparisonResult(parametersInteger[condition.paramName], condition.value, condition.comparison);
		if (result && edge->comparisonMode == TransitionCondition::OR)return true;
	}
	for (const auto& condition : edge->floatConditions) {

		if (parametersFloat.find(condition.paramName) == parametersFloat.end())continue;
		result = TransitionCondition::GetComparisonResult(parametersFloat[condition.paramName], condition.value, condition.comparison);
		if (result && edge->comparisonMode == TransitionCondition::OR)return true;
	}
	for (const auto& condition : edge->boolConditions) {

		if (parametersBool.find(condition.paramName) == parametersBool.end())continue;
		result = (parametersBool[condition.paramName] == condition.value);
		if (result && edge->comparisonMode == TransitionCondition::OR)return true;
	}
	for (const auto& condition : edge->triggerConditions) {

		if (parametersTrigger.find(condition.paramName) == parametersTrigger.end())continue;
		result = (parametersTrigger[condition.paramName] == true);
		if (result)
		{
			parametersTrigger[condition.paramName] = false;
			if (edge->comparisonMode == TransitionCondition::OR)return true;
		}
	}
	return result;
}

void Animator::Activate()
{
	Component::Activate();
	if (aninode)aninode->clock.Continue();
}

void Animator::Deactivate()
{
	Component::Deactivate();
	if (aninode)aninode->clock.Stop();
}