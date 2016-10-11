#include "EnemyAI.h"
#include"../NcgLua/NcgLua.hpp"
#include<algorithm>
#include<iterator>
void EnemyAI::CreateNodes()
{
	CreateNodeSafe(new MyNode{ 0,"obj1",{ 0.00, 0.00, -4.00 } });
	CreateNodeSafe(new MyNode{ 1,"obj2",{ 0.00, 0.00, 7.75 } });
	CreateNodeSafe(new MyNode{ 2,"obj3",{ -7.50, 0.00, 9.00 } });
	CreateNodeSafe(new MyNode{ 3,"obj4",{ -11.42, 0.00, 5.17 } });
	CreateNodeSafe(new MyNode{ 4,"obj5",{ -10.73, 0.00, -2.80 } });
	CreateNodeSafe(new MyNode{ 5,"obj6",{ -7.85, 0.00, -7.78 } });
	CreateNodeSafe(new MyNode{ 6,"obj7",{ -14.58, 0.00, -8.37 } });

	LinkNode({ 0, 1 }, 2, true);
	LinkNode({ 0, 4 }, 1, true);
	LinkNode({ 0, 5 }, 1, true);
	LinkNode({ 0, 6 }, 1, true);

	LinkNode({ 1, 2 }, 1, true);
	LinkNode({ 2, 3 }, 1, true);
	LinkNode({ 3, 4 }, 1, true);
	LinkNode({ 4, 5 }, 1, true);
	LinkNode({ 4, 6 }, 1, true);

}

std::vector<MyNode*> EnemyAI::GetNearNodeList(const DXVector3 & pCenter)
{
	std::vector<MyNode*> lNearNodeList;
	/*
		mNodes			= std::vector<Node*>
		lNearNodeList	= std::vector<MyNode*> 
		MyNode			= NodeÇåpè≥ÇµÇΩÉNÉâÉX
	*/
	lNearNodeList.reserve(mNodes.size());
	for (auto&lNode : mNodes) {
		lNearNodeList.push_back(static_cast<MyNode*>(lNode));
	}


	std::sort(lNearNodeList.begin(), lNearNodeList.end(), [&](MyNode* aLeft, MyNode* aRight) {
		float LengthL, LengthR;
		DXVector3 lTmpVector = aLeft->Position - pCenter;
		LengthL = lTmpVector.GetDistance();
		lTmpVector = aRight->Position - pCenter;
		LengthR = lTmpVector.GetDistance();

		return LengthL < LengthR;

	});

	return std::move(lNearNodeList);

}

void EnemyAI::Update(bool aTargeting, bool aTarget, bool aChargedEnergy)
{
	assert(mLuaAI);
	mLuaAI->Call(aTargeting,aTarget, aChargedEnergy);
	std::vector<std::string>lTmpPlan;
	mLuaAI->Return(lTmpPlan, 0);
	mAIType.clear();
	for (auto&lType : lTmpPlan) {
		mAIType.push_back(EnemyAIType::ConvertString(lType));
	}
	std::reverse(mAIType.begin(), mAIType.end());
}

EnemyAIType::Type EnemyAI::GetNowAI()
{
	if (mAIType.size() > 0) {
		return mAIType.back();
	}
	else {
		return EnemyAIType::Type::eUnKnown;
	}

}

bool EnemyAI::NextAI()
{
	if (mAIType.size() > 0) {
		mAIType.pop_back();
		return true;
	}
	else {
		return false;
	}
}

void EnemyAI::ClearAI()
{
	mAIType.clear();
}

EnemyAIType::Type EnemyAIType::ConvertString(const std::string & aStr)
{
	Type lType{ eUnKnown };

	if (aStr == sEnergyShot) {
		lType = eEnergyShot;
	}else if (aStr == sMovingHide) {
		lType = eMovingHide;
	}
	else if (aStr == sChargeEnergy) {
		lType = eChargeEnergy;
	}
	else if (aStr == sSearchTarget) {
		lType = eSearchTarget;
	}
	else if (aStr == sMoveToTarget) {
		lType = eMoveToTarget;
	}


	return lType;
}
