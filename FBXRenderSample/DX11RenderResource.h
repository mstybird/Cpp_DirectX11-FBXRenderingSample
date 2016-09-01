#pragma once
#include<d3dx9.h>
#include"DXTranslation.h"
#include"DXCamera.h"
#include"DXProjection.h"
#include"DXDisplay.h"
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

	DXWorld smWorld;
	DXCamera *smView;
	DXProjection *smProj;

};