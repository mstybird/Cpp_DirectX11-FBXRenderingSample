#include "StatusTeam.h"
#include<DXMath.hpp>
StatusTeam::StatusTeam(const eTeamType aTeamType):
	mTeamType(aTeamType)
{
}

StatusTeam::~StatusTeam()
{
	for (decltype(auto)lNode : mSpawnCharaNodes) {
		delete lNode;
	}
}

bool StatusTeam::IsMember(const GameObjectBase * aMember)
{
	for (decltype(auto)lMember : mMembers) {
		if (lMember == aMember) {
			return true;
		}
	}
	return false;
}

bool StatusTeam::IsCollisionBase(GameObjectBase * aCollider)
{
	DXVector3 lColliderPos;
	DXVector3 lBasePos;
	aCollider->GetWorld()->GetMatrix()->GetT(lColliderPos);
	mBase.GetWorld()->GetMatrix()->GetT(lBasePos);

	float lDistance = (lColliderPos - lBasePos).GetDistance();
	return lDistance < mBaseScale;
}

void StatusTeam::SetGoalIndex(const int aIndex)
{
	mGoalIndex = aIndex;
}

int StatusTeam::GetGoalIndex()
{
	return mGoalIndex;
}

StaticObject * StatusTeam::GetBase()
{
	return &mBase;
}

void StatusTeam::InitScore()
{
	mScore = 0;
}

int StatusTeam::GetScore()
{
	return mScore;
}

void StatusTeam::AddScore(const int aScore)
{
	mScore += aScore;
}

std::vector<GameObjectBase*>* StatusTeam::GetMembers()
{
	return &mMembers;
}

eTeamType StatusTeam::GetTeamType()
{
	return mTeamType;
}
