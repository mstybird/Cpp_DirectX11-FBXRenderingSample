#pragma once
#include<d3dx9.h>
/*
	メモ
	Sprite2D
	Sprite3D
	SpriteScreen
	C++11移行


*/
#include<memory>
class DXDisplay;
class DXWorld;
class DXCamera;
class DXProjection;
class DX11RenderResource {
public:

	DX11RenderResource();
	~DX11RenderResource();
	//描画に必要なビュー行列と射影行列が必要な場合必ず呼ぶ
	void InitRenderMatrix();
	std::weak_ptr<D3DXMATRIX>GetMatrixWorld();
	std::weak_ptr<D3DXMATRIX>GetMatrixView();
	std::weak_ptr<D3DXMATRIX>GetMatrixProjection();
	std::unique_ptr<D3DXMATRIX> GetMatrixWVP(const std::weak_ptr<DXDisplay>pDisplay);
	std::unique_ptr<D3DXMATRIX> GetMatrixWVP(const std::weak_ptr<D3DXMATRIX>pAddWorldMatrix, const std::weak_ptr<DXDisplay>pDisplay);

	std::weak_ptr<DXWorld>GetWorld();
	std::weak_ptr<DXCamera>GetCamera();
	std::weak_ptr<DXProjection>GetProjection();

private:
	std::shared_ptr<DXWorld>mWorld;
	std::shared_ptr<DXCamera>mView;
	std::shared_ptr<DXProjection>mProj;

};