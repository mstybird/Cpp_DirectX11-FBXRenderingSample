#include "StatusBase.h"

StatusBase::StatusBase() :
	mTargetChara{ nullptr },
	mEnergy{},
	mBall{ nullptr },
	mLive{CharaStateFlag::ALIVE}
{}
