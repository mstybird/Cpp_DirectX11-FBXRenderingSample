#include"BulletObject.h"
#include"MSBase3DShader.h"
#include"StatusBulletBase.h"
BulletObject::~BulletObject()
{
}

void BulletObject::SetBulletMesh(MSFbxManager & aSetMesh)
{
	mBulletMesh = &aSetMesh;
//	mCollisionMesh = &aSetMesh;
}

void BulletObject::UpdateStatus()
{
	mStatus->mInterval.AddPer(0.01f);
}


