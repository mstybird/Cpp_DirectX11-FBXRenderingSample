#include "MSCollisionBase.h"
#include"DXVector3.h"
#include"MSFbxManager.h"
#include"DX11FbxResource.h"
#include"DXVector3.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"DXCamera.h"
#include"DX11RenderResource.h"
#include<array>
#include<vector>
#include<map>
using std::pair;
using std::vector;


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


