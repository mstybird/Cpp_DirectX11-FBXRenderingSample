#include "DX11RenderResource.h"

DX11RenderResource::DX11RenderResource()
{
	mView = nullptr;
	mProj = nullptr;
}

DX11RenderResource::~DX11RenderResource()
{
	if (mView) {
		delete mView;
	}
	if (mProj) {
		delete mProj;
	}
}

void DX11RenderResource::InitRenderMatrix()
{
	mView = new DXCamera;
	mProj = new DXProjection;
}

D3DXMATRIX * DX11RenderResource::GetMatrixWorld()
{
	return mWorld.GetMatrix();
}

D3DXMATRIX * DX11RenderResource::GetMatrixView()
{
	return mView->GetMatrix();
}

D3DXMATRIX * DX11RenderResource::GetMatrixProjection()
{
	return mProj->GetMatrix();
}

D3DXMATRIX DX11RenderResource::GetMatrixWVP(DXDisplay*pDisplay)
{
	return *mWorld.GetMatrix() * *pDisplay->mUseCameraPtr->GetMatrix() * *pDisplay->mUseProjectionPtr->GetMatrix();
}

D3DXMATRIX DX11RenderResource::GetMatrixWVP(D3DXMATRIX * pAddWorldMatrix, DXDisplay * pDisplay)
{
	return *pAddWorldMatrix * *mWorld.GetMatrix() * 
		*pDisplay->mUseCameraPtr->GetMatrix() * 
		*pDisplay->mUseProjectionPtr->GetMatrix();
}

DXWorld * DX11RenderResource::GetWorld()
{
	return &mWorld;
}

DXCamera * DX11RenderResource::GetCamera()
{
	return mView;
}

DXProjection * DX11RenderResource::GetProjection()
{
	return mProj;
}
