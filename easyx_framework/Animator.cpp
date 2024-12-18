#include "Animator.h"
#include"World.h"
#include"ResourceManager.h"
#include"SpriteRenderer.h"
#include"Sprite.h"

//加载动画资源
void Animation::Load(std::string name, Vector2D delta)
{
	AnimationResource ani = mainWorld.resourcePool->AnimationFetch(name);
	
	num = ani.num;
	images = ani.images;
	offset = delta;
}

void Animation::Tick_()
{
	//如果不可循环则会静止在最后一帧(前提不是蒙太奇动画)
	if (index == num - 1 && !bLooping  && !bMontage)
	{
		clock.Stop(); return;
	}
	UpdateIndex();
	if (notification.find(index) != notification.end())notification[index].Execute();

	//若为蒙太奇动画，则播放一遍后立马跳出 
	//已经执行完一轮了 计时器第一次执行到这下标已经到0了
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

	//若无过渡条件，则播放一遍结束立马到下一个节点
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
 * 获取物体的渲染组件.
 * 
 */
void Animator::BeginPlay()
{
	if (rendererAttached)return;

	rendererAttached = pOwner->GetComponentByClass<SpriteRenderer>();
	if (rendererAttached)rendererAttached->animatorAttached = this;
}

///获取渲染器 进行渲染 并且进行条件触发
void Animator::Update(float deltaTime)
{
	//if (!rendererAttached)
	//{
	//	rendererAttached=pOwner->GetComponentByClass<SpriteRenderer>();		//获取渲染器
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

		///实现坐标位置对于动画是处于那个位置 一般是左上角点
		rendererAttached->spriteinfo.EndLoc = { sprite->getwidth(), sprite->getheight() };


		rendererAttached->DealImage();
	}

	//检查过渡条件
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
 * 插入动画资源到动画容器.
 * 
 * \param [in] name 动画名字
 * \param [in] ani 动画资源
 */
void Animator::insert(std::string name, Animation& ani)
{
	//动画资源不能为空
	if(ani.images)
	animations.insert({ name,ani });
	ani.animController = this;

}

/**
 * 设置当前播放的节点.
 * @remark (不建议直接使用)
 * \param [in] name 动画名字
 */
void Animator::SetNode(std::string name)
{
	//停止当前动画播放 防止资源占用 debug2024.9.20如果不加判定aninode是空就会出错
	if (aninode) { aninode->clock.Stop(); aninode->OnAnimExit.Execute(); }
	
	//设置节点
	aninode = &(animations.find(name)->second);
	aninode->index = 0;
	//防止节点上次已经被我们停止时钟
	aninode->clock.Continue();
	aninode->OnAnimEnter.Execute();
	
}

/**
 * 设置当前播放的节点.
 * @remark (不建议直接使用)
 * \param [in]node 动画接口类
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

///是否播放动画
void Animator::SetCalled(bool called)
{
	called ? aninode->clock.Continue() : aninode->clock.Stop();
}

void Animator::PlayMontage(std::string nodeName)
{
	if (aninode == &(animations.find(nodeName)->second))//若当前正在播放相同蒙太奇动画，则重置为第一帧
	{
		aninode->index = 0; return;
	}
	lastnode = aninode;
	SetNode(nodeName);
	aninode->bMontage = true;

	if (aninode->nexts.empty())
		aninode->OnMontageExit.Bind([this]()//若蒙太奇节点独立(无后继节点），则播放完毕后默认恢复之前的动画节点 
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