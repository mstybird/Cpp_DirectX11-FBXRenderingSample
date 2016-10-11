#include "EnemyAI.h"
#include"../NcgLua/NcgLua.hpp"
#include<algorithm>
#include<iterator>
void EnemyAI::CreateNodes()
{

	CreateNodeSafe(new MyNode{ 0,"obj0",{ 4.00,0.00,-6.75 } });
	CreateNodeSafe(new MyNode{ 1,"obj1",{ 6.75,0.00,-8.75 } });
	CreateNodeSafe(new MyNode{ 2,"obj2",{ 9.75,0.00,-5.75 } });
	CreateNodeSafe(new MyNode{ 3,"obj3",{ 11.75,0.00,-3.75 } });
	CreateNodeSafe(new MyNode{ 4,"obj4",{ 9.00,0.00,-0.50 } });
	CreateNodeSafe(new MyNode{ 5,"obj5",{ 6.25,0.00,2.50 } });
	CreateNodeSafe(new MyNode{ 6,"obj6",{ 2.75,0.00,0.25 } });
	CreateNodeSafe(new MyNode{ 7,"obj7",{ 0.50,0.00,-2.50 } });
	CreateNodeSafe(new MyNode{ 8,"obj8",{ -4.75,0.00,-2.50 } });
	CreateNodeSafe(new MyNode{ 9,"obj9",{ -8.25,0.00,-2.50 } });
	CreateNodeSafe(new MyNode{ 10,"obj10",{ -11.00,0.00,1.00 } });
	CreateNodeSafe(new MyNode{ 11,"obj11",{ -11.00,0.00,5.25 } });
	CreateNodeSafe(new MyNode{ 12,"obj12",{ -8.00,0.00,8.50 } });
	CreateNodeSafe(new MyNode{ 13,"obj13",{ -4.00,0.00,8.50 } });
	CreateNodeSafe(new MyNode{ 14,"obj14",{ -0.75,0.00,5.00 } });
	CreateNodeSafe(new MyNode{ 15,"obj15",{ 2.50,0.00,7.50 } });
	CreateNodeSafe(new MyNode{ 16,"obj16",{ 7.00,0.00,7.50 } });
	CreateNodeSafe(new MyNode{ 17,"obj17",{ 11.00,0.00,4.00 } });
	CreateNodeSafe(new MyNode{ 18,"obj18",{ 11.00,0.00,-8.50 } });
	CreateNodeSafe(new MyNode{ 19,"obj19",{ 11.00,0.00,7.50 } });
	CreateNodeSafe(new MyNode{ 20,"obj20",{ -16.25,0.00,5.25 } });
	CreateNodeSafe(new MyNode{ 21,"obj21",{ -16.25,0.00,-3.50 }});

	
	LinkNode({ 0, 1 }, 1, true);
	LinkNode({ 0, 7 }, 1, true);
	LinkNode({ 0, 8 }, 1, true);
	LinkNode({ 1, 2 }, 1, true);
	LinkNode({ 2, 3 }, 1, true);
	LinkNode({ 2, 18 }, 1, true);
	LinkNode({ 3, 4 }, 1, true);
	LinkNode({ 3, 17 }, 1, true);
	LinkNode({ 3, 18 }, 1, true);
	LinkNode({ 4, 5 }, 1, true);
	LinkNode({ 4, 19 }, 1, true);
	LinkNode({ 5, 6 }, 1, true);
	LinkNode({ 5, 16 }, 1, true);
	LinkNode({ 5, 19 }, 1, true);
	LinkNode({ 6, 7 }, 1, true);
	LinkNode({ 6, 14 }, 1, true);
	LinkNode({ 7, 8 }, 1, true);
	LinkNode({ 8, 9 }, 1, true);
	LinkNode({ 8, 14 }, 1, true);
	LinkNode({ 9, 10 }, 1, true);
	LinkNode({ 9, 11 }, 1, true);
	LinkNode({ 10, 21 }, 1, true);
	LinkNode({ 11, 12 }, 1, true);
	LinkNode({ 11, 20 }, 1, true);
	LinkNode({ 11, 21 }, 1, true);
	LinkNode({ 12, 13 }, 1, true);
	LinkNode({ 12, 20 }, 1, true);
	LinkNode({ 13, 14 }, 1, true);
	LinkNode({ 13, 15 }, 1, true);
	LinkNode({ 14, 15 }, 1, true);
	LinkNode({ 15, 16 }, 1, true);
	LinkNode({ 16, 19 }, 1, true);

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
