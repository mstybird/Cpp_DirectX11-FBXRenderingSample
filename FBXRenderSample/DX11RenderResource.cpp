#include "DX11RenderResource.h"

DX11RenderResource::DX11RenderResource()
{
	smView = nullptr;
	smProj = nullptr;
}

DX11RenderResource::~DX11RenderResource()
{
	if (smView) {
		delete smView;
	}
	if (smProj) {
		delete smProj;
	}
}

void DX11RenderResource::InitRenderMatrix()
{
	smView = new DXCamera;
	smProj = new DXProjection;
}

D3DXMATRIX * DX11RenderResource::GetMatrixWorld()
{
	return smWorld.GetMatrix();
}

D3DXMATRIX * DX11RenderResource::GetMatrixView()
{
	return smView->GetMatrix();
}

D3DXMATRIX * DX11RenderResource::GetMatrixProjection()
{
	return smProj->GetMatrix();
}

D3DXMATRIX DX11RenderResource::GetMatrixWVP(DXDisplay*pDisplay)
{
	return *smWorld.GetMatrix() * *pDisplay->mUseCameraPtr->GetMatrix() * *pDisplay->mUseProjectionPtr->GetMatrix();
}
