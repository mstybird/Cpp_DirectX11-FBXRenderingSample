#pragma once
#include<d3dx9.h>
/*
	メモ
	Sprite2D
	Sprite3D
	SpriteScreen
	C++11移行


*/
#include"MSFbxObject.h"
#include<memory>
#include<vector>
class DXDisplay;
class DXWorld;
class DXCamera;
class DXProjection;
class DXMatrix;
class MSCollisionSphere;
class DX11RenderResource {
public:

	DX11RenderResource();
	~DX11RenderResource();
	//描画に必要なビュー行列と射影行列が必要な場合必ず呼ぶ
	void InitRenderMatrix();
	std::weak_ptr<D3DXMATRIX>GetMatrixWorld();
	std::weak_ptr<D3DXMATRIX>GetMatrixView();
	std::weak_ptr<D3DXMATRIX>GetMatrixProjection();
	std::unique_ptr<D3DXMATRIX> GetMatrixWVP(const DXDisplay& pDisplay);
	std::unique_ptr<D3DXMATRIX> GetMatrixWVP(const D3DXMATRIX& pAddWorldMatrix, const DXDisplay& pDisplay);


	void SetWorld(const DXWorld&pWorld);
	void SetCamera(const DXCamera&pCamera);
	void SetProjection(const DXProjection&pProjection);
	std::weak_ptr<DXWorld>GetWorld();
	std::weak_ptr<DXCamera>GetCamera();
	std::weak_ptr<DXProjection>GetProjection();

	//境界球の設定
	void SetCollisionSphere(const std::shared_ptr<std::vector<std::vector<MSCollisionSphere>>>&pCollisions);
	//指定したリソースとの衝突判定
	bool CollisionSphere( std::shared_ptr<DX11RenderResource>&pResource);
	void GetCollisionSphere(std::weak_ptr<std::vector<std::vector<MSCollisionSphere>>>&pOutCollisions);
	std::shared_ptr<MSFbxObject> mMesh;
private:
	//ワールド行列
	std::shared_ptr<DXWorld>mWorld;
	//ビュー行列
	std::shared_ptr<DXCamera>mView;
	//射影行列
	std::shared_ptr<DXProjection>mProj;
	//境界球コリジョン参照
	std::weak_ptr<std::vector<std::vector<MSCollisionSphere>>>mCollisions;



};