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
#include<DXMath.hpp>
#include<memory>
#include<vector>
class DXDisplay;
class DXMatrix;
class MSCollisionSphere;
class DX11RenderResource {
public:

	DX11RenderResource();
	~DX11RenderResource();
	//描画に必要なビュー行列と射影行列が必要な場合必ず呼ぶ
	void InitRenderMatrix();
	D3DXMATRIX* GetMatrixWorld();
	D3DXMATRIX* GetMatrixView();
	D3DXMATRIX* GetMatrixProjection();
	void GetMatrixWVP(D3DXMATRIX* pOut, const DXDisplay& pDisplay);
	void GetMatrixWVP(D3DXMATRIX* pOut, const D3DXMATRIX& pAddWorldMatrix, const DXDisplay& pDisplay);


	void SetWorld(const DXWorld&pWorld);
	void SetCamera(const DXCamera&pCamera);
	void SetProjection(const DXProjection&pProjection);
	DXWorld* GetWorld();
	DXCamera* GetCamera();
	DXProjection* GetProjection();

	//境界球の設定
	void SetCollisionSphere(const std::shared_ptr<std::vector<std::vector<MSCollisionSphere>>>&pCollisions);
	//指定したリソースとの衝突判定
	bool CollisionSphere( std::shared_ptr<DX11RenderResource>&pResource);
	void GetCollisionSphere(std::weak_ptr<std::vector<std::vector<MSCollisionSphere>>>&pOutCollisions);

	//メッシュ数を取得
	int GetMeshCount();
	//メッシュに対するサブメッシュ数を取得
	int GetSubMeshCount(int aMeshIndex);
	//メッシュを取得
	MSFbxObject* GetMesh();
	//透過率配列を取得
	std::vector<float>*GetTransVector();
private:
	//メッシュデータ
	std::shared_ptr<MSFbxObject> mMesh;
	//透明度情報(Mesh<SubMesh>)
	std::vector<float>mTransparents;


	//ワールド行列
	DXWorld mWorld;
	//ビュー行列
	DXCamera mView;
	//射影行列
	DXProjection mProj;
	//境界球コリジョン参照
	std::weak_ptr<std::vector<std::vector<MSCollisionSphere>>>mCollisions;



};