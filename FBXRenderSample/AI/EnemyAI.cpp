#include "EnemyAI.h"
#include"../NcgLua/NcgLua.hpp"
#include"../StatusField.h"
#include<algorithm>
#include<iterator>

EnemyAI::~EnemyAI()
{
}

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
	bool aBallHoldField,
	bool aBallHoldAlly,
	bool aBallHoldEnemy,
	bool aBallHoldMe,

	//EnemyState
	bool aInSigntEnemy,
	bool aLockonEnemy,
	bool aBallHoldTarget,

	//AllyState
	bool aInSightAlly,
	bool aBallAllyNear,
	bool aAllyNear,

	//Other
	bool aChargedEnergy)
{
	assert(mLuaAI);

	mLuaAI->Call(
		aBallHoldField,
		aBallHoldAlly,
		aBallHoldEnemy,
		aBallHoldMe,

		//EnemyState
		aInSigntEnemy,
		aLockonEnemy,
		aBallHoldTarget,

		//AllyState
		aInSightAlly,
		aBallAllyNear,
		aAllyNear,

		//Other
		aChargedEnergy
	);

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

	if (aStr == sMoveToBall) {
		lType = eMoveToBall;
	}
	else if (aStr == sMoveToGoal) {
		lType = eMoveToGoal;
	}
	else if (aStr == sMoveToHide) {
		lType = eMoveToHide;
	}
	else if (aStr == sChargeEnergy) {
		lType = eChargeEnergy;
	}
	else if (aStr == sInSightAttack) {
		lType = eInSightAttack;
	}
	else if (aStr == sMoveToBallHoldAlly) {
		lType = eMoveToBallHoldAlly;
	}
	else if (aStr == sSearchEnemyShort) {
		lType = eSearchEnemyShort;
	}
	else if (aStr == sSerachEnemyAll) {
		lType = eSerachEnemyAll;
	}
	else if (aStr == sMoveToLookingTarget) {
		lType = eMoveToLookingTarget;
	}
	else if (aStr == sMoveToBallTarget) {
		lType = eMoveToBallTarget;
	}
	else if (aStr == sSearchForEnemyArea) {
		lType = eSearchForEnemyArea;
	}
	else if (aStr == sSearchForAllyArea) {
		lType = eSearchForAllyArea;
	}



	return lType;
}
