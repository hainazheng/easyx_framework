#include "FSM.h"
#include"stack"
#include"Animator.h"

//�Ƴ�ĳ���ڵ㣬Ĭ���Ƴ������ӽڵ� ����ɾ�������ӽڵ�
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

	//1.���޹����������б� ����2.�Ѿ�������������ִ��һ����Ϊ����������һ���ڵ�
	for (auto edge : normalstatus->nexts)
	{
		if ((edge->IsUnconditional()) || CheckConditions(edge))
		{
			TransitionState(edge->end);
			break;
		}
	}

	////ִ�е�ǰ״̬����Ϊ
	//normalstatus->act();
}

//���ĳ���ڵ���״̬
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

		//��������̫��ֱ�ӱ���ƥ��
		if (path_p.empty() || path_q.empty())return nullptr;
		//�õ���С����
		int min_size = path_p.size() > path_q.size() ? path_q.size() : path_p.size();
		//�洢ǰһ������
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

//����״̬��
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

///�ڵ�ɾ��֮���ӽڵ��������ɭ��   
/// @remark �����븴��ɾ��ǰ�Ͱ󶨵������ڵ��ϣ�����״̬��Ĭ��ɾ�������ӽڵ� 
/// @remark ״̬�����ֻ��һ���������״̬��������ɭ��
void FSMItems::Destory()
{
	
	for (auto& it : children)
	{
		it->parent = nullptr;
		 
	}
	if(parent)parent->children.erase(this);
	delete this;						//2024.11.10 Debug ɾ���ڵ��ʱ�����delete��delete����������������������������delete��ѭ��ִ��
}



bool StatusSort::operator()(const FSMItems* a, const FSMItems* b) const
{
	if(a->GetStatus() == b->GetStatus())return a<b;
	else {
		return a->GetStatus() < b->GetStatus();
	}
	
}
