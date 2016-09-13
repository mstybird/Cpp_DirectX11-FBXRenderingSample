#include "MSCollision.h"
#include"DXVector3.h"
#include"MSFbxManager.h"
#include"DX11FbxResource.h"
#include"DXMatrix.h"
MSCollisionSphere::MSCollisionSphere():
	mRadius{0}
	//mCenter{std::make_unique<DXVector3>()}
{
}

void MSCollisionSphere::CreateCollision(const FBXModelData&pMesh, const DXMatrix&pGlobalPosition)
{
	D3DXComputeBoundingSphere((const D3DXVECTOR3*)pMesh.Data.data(), pMesh.Data.size(), sizeof(FbxVertex), mCenter.get(), &mRadius);
	//îªíËògèkè¨
	mRadius *= 0.7f;
	mGlobalPosition =std::make_unique<DXMatrix>(pGlobalPosition);
}

bool MSCollisionSphere::Collision(MSCollisionSphere & pCollision)
{
	return false;
}

MSCollisionBase::MSCollisionBase():
	mGlobalPosition{std::make_unique<DXMatrix>()}
{
}

MSCollisionBase::MSCollisionBase(const MSCollisionBase & _co)
{
	mGlobalPosition = std::make_unique<DXMatrix>(*this->mGlobalPosition);
}

MSCollisionBase::~MSCollisionBase()
{
}
