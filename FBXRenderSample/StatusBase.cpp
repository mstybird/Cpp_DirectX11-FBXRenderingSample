#include "StatusBase.h"

StatusBase::StatusBase() :
	mTarget{ nullptr },
	mEnergy{},
	mBall{ nullptr },
	mLive{CharaStateFlag::ALIVE}
{}
