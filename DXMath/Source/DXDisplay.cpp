#include "DXDisplay.h"
#include"DXCamera.h"
#include"DXProjection.h"

void DXDisplay::SetRenderTarget(DXCamera* pCamera, DXProjection* pProjection)
{
	mUseCamera = pCamera;
	mUseProjection = pProjection;
}

DXCamera* DXDisplay::GetCamera() const
{
	return mUseCamera;
}

DXProjection* DXDisplay::GetProjection() const
{
	return mUseProjection;
}
