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
	aCollider->GetWorld()->GetMatrix().lock()->GetT(lColliderPos);
	mBase.GetWorld()->GetMatrix().lock()->GetT(lBasePos);

	float lDistance = (lColliderPos - lBasePos).GetDistance();
	return lDistance < mBaseScale;
}
