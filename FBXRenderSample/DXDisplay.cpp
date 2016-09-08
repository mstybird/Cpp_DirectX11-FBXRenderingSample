#include "DXDisplay.h"
#include"DXCamera.h"
#include"DXProjection.h"

void DXDisplay::SetRenderTarget(DXCamera * pCamera, DXProjection * pProjection)
{
	mUseCameraPtr = pCamera;
	mUseProjectionPtr = pProjection;
}

DXCamera * DXDisplay::GetCamera() const
{
	return mUseCameraPtr;
}

DXProjection * DXDisplay::GetProjection() const
{
	return mUseProjectionPtr;
}
