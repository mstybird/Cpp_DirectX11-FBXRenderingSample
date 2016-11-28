#include "StatusTeam.h"

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
