#include "StatusTeam.h"

bool StatusTeam::IsMember(const GameObjectBase * aMember)
{
	for (decltype(auto)lMember : mMembers) {
		if (lMember == aMember) {
			return true;
		}
	}
	return false;
}
