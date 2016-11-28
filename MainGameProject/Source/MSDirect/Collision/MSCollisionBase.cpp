#include "MSCollisionBase.h"
#include"MSFbxManager.h"
#include<DX11FBXLoader.hpp>
#include"DX11RenderResource.h"
#include"DXMath.hpp"

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


