#include "EnemyAI.h"
#include"../NcgLua/NcgLua.hpp"
#include"../StatusField.h"
#include<algorithm>
#include<iterator>

void EnemyAI::CreateNodes(StatusField&aField)
{
	mNodes = std::move(aField.GetFieldNodesClone());
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

void EnemyAI::Update(
	bool aTargeting,
	bool aTarget,
	bool aChargedEnergy,
	bool aTargetHoldBall,
	bool aHoldingBall,
	bool aBallIsField)
{
	assert(mLuaAI);
	mLuaAI->Call(aTargeting,aTarget, aChargedEnergy,aTargetHoldBall,aHoldingBall,aBallIsField);
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
	else if (aStr == sMoveToBall) {
		lType = eMoveToBall;
	}
	else if (aStr == sMoveToBallTarget) {
		lType = eMoveToBallTarget;
	}
	else if (aStr == sMoveToGoal) {
		lType = eMoveToGoal;
	}

	return lType;
}
