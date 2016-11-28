#include "MSGravity.h"
#include"MSProgressData.hxx"
MSGravity::MSGravity()
{
	mGravity = std::make_unique<MSProgress>();
}

MSGravity::~MSGravity()
{
}

void MSGravity::Initialize(bool aIsGravity, float aMaxGravity, float aMinGravity, float aNowGravity, float aRepulsion, float aAttraction)
{
	mIsGravity = aIsGravity;
	mGravity->Set(aMaxGravity, aMinGravity, aNowGravity);
	mRepulsion = aRepulsion;
	mAttraction = aAttraction;
	mFrameAttraction = 0.0f;
}

void MSGravity::UpdateUpGravity()
{
	mFrameAttraction = mRepulsion;
}

void MSGravity::UpdateDownGravity()
{
	mFrameAttraction = -mRepulsion;

}

void MSGravity::UpdateGravity()
{
	mGravity->SubFix(mFrameAttraction);
	mFrameAttraction -= mAttraction;
}

float MSGravity::GetGravity()
{
	return mGravity->GetNow();
}
