#include "DXDisplay.h"
#include"DXCamera.h"
#include"DXProjection.h"

void DXDisplay::SetRenderTarget(const std::weak_ptr<DXCamera>pCamera, const std::weak_ptr<DXProjection>pProjection)
{
	mUseCamera = pCamera;
	mUseProjection = pProjection;
}

std::weak_ptr<DXCamera> DXDisplay::GetCamera() const
{
	return mUseCamera;
}

std::weak_ptr<DXProjection> DXDisplay::GetProjection() const
{
	return mUseProjection;
}
