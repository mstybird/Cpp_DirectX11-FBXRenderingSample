#include "StatusEnemy.h"

EnemyStatus::EnemyStatus() :
	mTargetting{ false },
	mLockOnType{EnemyLockOnType::UNKNOWN},
	mInitMoveToBall{false},
	mInitMoveToGoal{false},
	mInitMoveToBallTarget{false}
{};

