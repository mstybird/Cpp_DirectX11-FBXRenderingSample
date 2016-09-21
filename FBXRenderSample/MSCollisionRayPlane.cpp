#include "MSCollisionRayPlane.h"
#include"DXVector3.h"
#include"DX11Resrouce.h"
#include"DX11RenderResource.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"MSFbxManager.h"
#include"DX11FbxResource.h"
MSCollisionRayPlane::MSCollisionRayPlane()
{
	vPoint = new DXVector3;
	vDirection = new DXVector3;
	vPos = new DXVector3;
}

MSCollisionRayPlane::~MSCollisionRayPlane()
{
	SAFE_DELETE(vPoint);
	SAFE_DELETE(vDirection);
	SAFE_DELETE(vPos);
}

bool MSCollisionRayPlane::Collision(DX11RenderResource & pRayPosition, const DXVector3& pRayLength, DX11RenderResource & pRayTarget, MSFbxManager & pFbxTarget)
{
	//レイの始点と終点
	DXVector3 vP[2];

	//レイの始点を取得し、終点を計算する
	{
		pRayPosition.GetWorld().lock()->GetMatrix().lock()->GetT(vP[0]);
		const DXVector3& lRotateWorld = *pRayPosition.GetWorld().lock()->mRotationCenter;
		DXMatrix lRotateMatrix;
		DXMatrix lTransMatrix;
		lRotateMatrix.RotationXYZ(lRotateWorld, TYPE_ANGLE::DEGREE);
		lTransMatrix.Translation(vP[0]);
		D3DXVec3TransformCoord(&vP[1], &pRayLength, &(lRotateMatrix*lTransMatrix));
	}

	{
		using FbxPolygon = std::array<FbxVertex, 3>;
		using FbxPtrPolygon = std::array<FbxVertex*, 3>;

		DXMatrix&lResourceMatrix = *pRayTarget.GetWorld().lock()->GetMatrix().lock();
		auto lMeshList = pFbxTarget.GetMeshData();


		DXVector3 lPosition[3];		//レイ判定使う最終的な3頂点
		for (auto&lMesh : *lMeshList) {
			//変換行列の合成
			//メッシュに定義されている行列を取得
			DXMatrix lGlobalMatrix{ *lMesh->mWorld.get() };
			//合成
			lGlobalMatrix = lGlobalMatrix * lResourceMatrix;
			for (auto&lSubMesh : lMesh->subMesh) {
				for (int i = 0; i < lSubMesh->PolygonCount; i++) {
					FbxPtrPolygon lPtrPolygon;
					lSubMesh->GetPolygon(lPtrPolygon, i);

					//行列を使って頂点位置の修正
					DXMatrix lPositionMatrix[3];
					for (int i = 0; i < 3; ++i) {
						lPositionMatrix[i].Translation(lPtrPolygon[i]->Pos);
						lPositionMatrix[i] *= lGlobalMatrix;
						lPositionMatrix[i].GetT(lPosition[i]);
					}


					//判定
					{
						D3DXPLANE p;
						D3DXPlaneFromPoints(
							&p,
							&lPosition[0],
							&lPosition[1],
							&lPosition[2]
							);
						float t;
						t = -((p.a * vP[1].x) + (p.b*vP[1].y) + (p.c*vP[1].z) + p.d) /
							(((p.a*vP[0].x) + (p.b*vP[0].y) + (p.c*vP[0].z)) - ((p.a*vP[1].x) + (p.b*vP[1].y) + (p.c*vP[1].z)));
						// t が0から1の間であるなら交差していることになる
						if (t >= 0 && t <= 1.0f) {

							//交差していた場合、交点座標を得る
							DXVector3 lCrossPoint;
							//それぞれのベクトルの長さに媒介変数をかけて、交差点を計算する
							lCrossPoint.x = t*vP[0].x + (1 - t)*vP[1].x;
							lCrossPoint.y = t*vP[0].y + (1 - t)*vP[1].y;
							lCrossPoint.z = t*vP[0].z + (1 - t)*vP[1].z;

							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool MSCollisionRayPlane::IsInside( DXVector3 & pvI,  DXVector3 & pvA,  DXVector3 & pvB,  DXVector3 & pvC)
{
	//辺ベクトル(辺の長さ)
	DXVector3 vAB;
	DXVector3 vBC;
	DXVector3 vCA;
	vAB = pvB - pvA;
	vBC = pvC - pvB;
	vCA = pvA - pvC;

	//辺ベクトルと頂点から交点へ向かうベクトルとのそれぞれの外積用
	DXVector3 vCrossAB;
	DXVector3 vCrossBC;
	DXVector3 vCrossCA;

	float dotAB;
	float dotBC;
	float dotCA;

	/*
		有限平面との当たり判定
	
	*/

}
