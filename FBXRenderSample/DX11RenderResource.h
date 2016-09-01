#pragma once
#include<d3dx9.h>
#include"DXTranslation.h"
#include"DXCamera.h"
#include"DXProjection.h"
class DX11RenderResource {
public:
	D3DXMATRIX*GetMatrixWorld();
	D3DXMATRIX*GetMatrixView();
	D3DXMATRIX*GetMatrixProjection();

	DXWorld smWorld;
	DXCamera smView;
	DXProjection smProj;

	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
};