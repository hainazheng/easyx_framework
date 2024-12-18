#include "FSM.h"
#include"stack"
#include"Animator.h"

//移除某个节点，默认移除所有子节点 建议删除所有子节点
void FSM::RemoveStatus(std::string name,bool delchild)
{
	FSMItems* it = FindStatusByName(name);
	if (!it)return;

	if (delchild) { Clear(name); return; }
	delete it;
}

void FSM::process_Update(FSMItems* item)
{
	if (!item)return;
	if (item->parent != nullptr)
	{
		process_Update(item->parent);
	}
	item->act();
}

void FSM::process_Enter(FSMItems* item, FSMItems* end )
{
	if (!item)return;
	if (item->parent != end)
	{
		process_Enter(item->parent, end);
	}
	item->enter();
}

void FSM::process_Exit(FSMItems* item, FSMItems* end )
{
	if (!item)return; item->exit();
	if (item->parent != end)
	{

		process_Exit(item->parent, end);

	}
}

void FSM::Update(float deltaTime)
{
	Component::Update(deltaTime);
	process_Update(normalstatus);

	//1.若无过渡条件且有边 或者2.已经符合条件，则执行一遍行为结束立马到下一个节点
	for (auto edge : normalstatus->nexts)
	{
		if ((edge->IsUnconditional()) || CheckConditions(edge))
		{
			TransitionState(edge->end);
			break;
		}
	}

	////执行当前状态的行为
	//normalstatus->act();
}

//清空某个节点子状态
void FSM::Clear(std::string name)
{
	std::stack<FSMItems*>status_to_delete;
	FSMItems* it = FindStatusByName(name);
	status_to_delete.push(it);
	while (!status_to_delete.empty())
	{
		FSMItems* current_status = status_to_delete.top();
		status_to_delete.pop();

		if (!current_status->children.empty())
		{
			for (auto& child : current_status->children)
				status_to_delete.push(child);
		}
		UnregisterStatus(current_status);
		delete current_status;

	}
}

FSMItems* FSM::lowestCommonAncestor(FSMItems* root, FSMItems* p, FSMItems* q)
{
	
		if (!root) return root;

		std::list<FSMItems*> path_p = GetPath(p);
		std::list<FSMItems*> path_q = GetPath(q);

		//数量不会太多直接暴力匹配
		if (path_p.empty() || path_q.empty())return nullptr;
		//得到最小数量
		int min_size = path_p.size() > path_q.size() ? path_q.size() : path_p.size();
		//存储前一个变量
		FSMItems* temp = root;
		for (int i = 0; i < min_size; i++)
		{
			if (path_p.back() != path_q.back())
			{
				return temp;
			}
			temp = path_p.back();
			path_p.pop_back();
			path_q.pop_back();
		}
		return nullptr;
	
}

//销毁状态机
void FSM::Destruct()
{

	std::stack<FSMItems*>status_to_delete;

	status_to_delete.push(this->root);
	while (!status_to_delete.empty())
	{
		FSMItems* current_status = status_to_delete.top();
		status_to_delete.pop();

		if (!current_status->children.empty())
		{
			for (auto& child : current_status->children)
				status_to_delete.push(child);
		}
		UnregisterStatus(current_status);

		/*delete current_status;*/
		current_status->Destory();

	}
}

void FSM::AddParamater(std::string paramName, ParamType type)
{
	if (type == ParamType::Integer)
	{
		parametersInteger.insert({ paramName,int(0) });
		
	}
	else if (type == ParamType::Float)
	{
		parametersFloat.insert({ paramName,float(0.0) });
	}
	else if (type == ParamType::Bool)
	{
		parametersBool.insert({ paramName,bool(false) });
	}
	else
	{
		parametersTrigger.insert({ paramName,false });
	}
}

void FSM::SetTrigger(std::string paramName)
{
	if (parametersTrigger.find(paramName) != parametersTrigger.end())
	{
		parametersTrigger[paramName] = true;
	}
}

bool FSM::CheckConditions(FSMEdge* edge)
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

///节点删除之后子节点独立出现森林   
/// @remark 建议想复用删除前就绑定到其他节点上，否则状态机默认删除所有子节点 
/// @remark 状态机最好只是一棵树，多个状态机可以是森林
void FSMItems::Destory()
{
	
	for (auto& it : children)
	{
		it->parent = nullptr;
		 
	}
	if(parent)parent->children.erase(this);
	delete this;						//2024.11.10 Debug 删除节点的时候调用delete，delete会调用析构函数，析构函数会继续delete会循环执行
}



bool StatusSort::operator()(const FSMItems* a, const FSMItems* b) const
{
	if(a->GetStatus() == b->GetStatus())return a<b;
	else {
		return a->GetStatus() < b->GetStatus();
	}
	
}
