#pragma once
#include<d3dx9.h>
#include"DXWorld.h"
#include"DXCamera.h"
#include"DXProjection.h"
#include"DXDisplay.h"
struct D3DXMATRIX;
class DX11RenderResource {
public:
	DX11RenderResource();
	~DX11RenderResource();
	//�`��ɕK�v�ȃr���[�s��Ǝˉe�s�񂪕K�v�ȏꍇ�K���Ă�
	void InitRenderMatrix();
	D3DXMATRIX*GetMatrixWorld();
	D3DXMATRIX*GetMatrixView();
	D3DXMATRIX*GetMatrixProjection();
	D3DXMATRIX GetMatrixWVP(DXDisplay*pDisplay);
	D3DXMATRIX GetMatrixWVP(D3DXMATRIX*pAddWorldMatrix, DXDisplay*pDisplay);

	DXWorld*GetWorld();
	DXCamera*GetCamera();
	DXProjection*GetProjection();

private:
	DXWorld mWorld;
	DXCamera *mView;
	DXProjection *mProj;

};