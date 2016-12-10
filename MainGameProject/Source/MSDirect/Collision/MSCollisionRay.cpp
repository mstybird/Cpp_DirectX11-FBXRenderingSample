#include "MSCollisionRay.h"
#include"MSFbxManager.h"
#include<DX11FBXLoader.hpp>
#include"DXMath.hpp"
#include"DX11RenderResource.h"
#include"MSFbxObject.h"
#include<array>
#include<vector>
#include<map>



MSCollisionRay::MSCollisionRay() :
	mRayDirection{ std::make_shared<DXVector3>() }
{
}

void MSCollisionRay::SetRay(const DXVector3 & pRayDirection)
{
	*mRayDirection = pRayDirection;
}


void MSCollisionRay::SetRay(const DXCamera & pRayCamera)
{
	pRayCamera.CreateRay(*mRayDirection.get(), { 0.0f,0.0f,1.0f });
}

bool MSCollisionRay::Collision(const MSCollisionRay & pRay, DX11RenderResource&pRayPosition, DX11RenderResource & pResource, MSFbxObject & pFbxObject)
{
	//1つのポリゴンの3つの頂点
	std::array<FbxVertex*, 3>lPolygon;
	//リソースのワールド座標
	DXVector3 lPosition[3];
	//ポリゴン取得


	float lDistance;

	


	auto lmeshList = pFbxObject.GetCurrentMeshData();
	DXMatrix lResourceMatrix = *pResource.GetWorld().lock()->GetMatrix().lock();
	for (auto&lMesh : *lmeshList) {

		//変換行列の合成
		DXMatrix lGlobalMatrix{ *lMesh->mWorld.get() };
		lGlobalMatrix = lGlobalMatrix * lResourceMatrix;
		for (auto&lSubMesh : lMesh->subMesh) {
			for (uint32_t i = 0; i < lSubMesh->PolygonCount; i++) {
				//判定を行うポリゴンを取得
				lSubMesh->GetPolygon(lPolygon, i);

				//リソース上の位置を与えた行列を作成する
				DXMatrix lPositionMatrix[3];

				for (int i = 0; i < 3; ++i) {
					lPositionMatrix[i].Translation(lPolygon[i]->Pos);
					lPositionMatrix[i] *= lGlobalMatrix;
					lPositionMatrix[i].GetT(lPosition[i]);
				}
				//レイ判定
				DXVector3 lRayPos;
				pRayPosition.GetCamera().lock()->GetEyeT(lRayPos);
				//				pRayPosition.GetWorld().lock()->GetMatrix().lock()->GetT(lRayPos);
				if (D3DXIntersectTri(
					&lPosition[0],
					&lPosition[1],
					&lPosition[2],
					&lRayPos,
					pRay.mRayDirection.get(),
					NULL,
					NULL,
					&lDistance
					)) {
					return true;
				}
			}
		}
	}
	return false;
}

float MS2CollisionRay::Collision(DXCamera * aCamera, GameObjectBase * aCollision)
{
	return 0.0f;
}
