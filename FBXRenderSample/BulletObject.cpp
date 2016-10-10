#include"BulletObject.h"
#include"MSBase3DShader.h"
BulletObject::~BulletObject()
{
}

void BulletObject::SetBulletMesh(MSFbxManager & aSetMesh)
{
	mBulletMesh = &aSetMesh;
}


