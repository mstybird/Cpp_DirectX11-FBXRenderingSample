#include "MSCollisionRayPlane.h"
#include"DXVector3.h"
#include"DX11Resrouce.h"
#include"DX11RenderResource.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"MSFbxManager.h"
#include"DX11FbxResource.h"
#include<map>
using std::pair;
MSCollisionRayPlane::MSCollisionRayPlane():
	mFramePosition{std::make_unique<DXVector3>()}
{
}

MSCollisionRayPlane::~MSCollisionRayPlane()
{
}

bool MSCollisionRayPlane::Collision(DX11RenderResource & pRayPosition, const DXVector3& pRayLength, DX11RenderResource & pRayTarget, MSFbxManager & pFbxTarget, float& pOutDistance, DXVector3&pOutNormal)
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



		//距離を初期化
		pOutDistance = FLT_MAX;
		DXVector3 lPosition[3];		//レイ判定使う最終的な3頂点
		for (auto&lMesh : *lMeshList) {
			//変換行列の合成
			//メッシュに定義されている行列を取得
			DXMatrix lGlobalMatrix{ *lMesh->mWorld.get() };
			//合成
			lGlobalMatrix = lGlobalMatrix * lResourceMatrix;
			//サブメッシュ単位でループ
			for (auto&lSubMesh : lMesh->subMesh) {
				//ポリゴン単位でループ
				for (int i = 0; i < lSubMesh->PolygonCount; i++) {
					FbxPtrPolygon lPtrPolygon;
					//メッシュを構成する一つのポリゴンを取得する
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

							if (IsInside(
								lCrossPoint,
								lPosition[0],
								lPosition[1],
								lPosition[2]
							)) {
								//レイ発射位置から交点を引いたベクトルの距離が、交点との距離
								float lTmpDistance = (vP[0] - lCrossPoint).GetDistance();
								//既に衝突したポリゴンより近ければ更新
								if (pOutDistance > lTmpDistance) {
									pOutDistance = lTmpDistance;
									
									pOutNormal.x = p.a;
									pOutNormal.y = p.b;
									pOutNormal.z = p.c;
								}
							}
						}
					}
				}
			}
		}
		if (pOutDistance == FLT_MAX)return false;
	}
	return true;
}

bool MSCollisionRayPlane::Collision(DX11RenderResource & pRayPosition, const float & pRayLength, DX11RenderResource & pRayTarget, MSFbxManager & pFbxTarget, float & pOutDistance, DXVector3&pOutNormal)
{
	//レイの方向を計算する
	DXVector3 lRayDirection;
	pRayPosition.GetWorld().lock()->GetMatrix().lock()->GetT(lRayDirection);
	lRayDirection -= *mFramePosition;
	//方向を確定
	lRayDirection.Normalize();
	//距離を加えてレイの長さを計算する
	lRayDirection *= pRayLength;
	//このレイを使って判定
	if(Collision(
		pRayPosition,
		lRayDirection,
		pRayTarget,
		pFbxTarget,
		pOutDistance,
		pOutNormal
	)) {
		return true;
	}
	return false;
}

void MSCollisionRayPlane::SetFramePosition(DX11RenderResource & pRayPosition)
{
		pRayPosition.GetWorld().lock()->GetMatrix().lock()->GetT(*mFramePosition);
}

void MSCollisionRayPlane::Slip(DXVector3&pOutSlipVector,DX11RenderResource & pUpdPosition, DXVector3&pNormal)
{
	//移動ベクトル(滑りベクトルの計算
	//レイの方向を計算する
	DXVector3 lRayDirection;
	pUpdPosition.GetWorld().lock()->GetMatrix().lock()->GetT(lRayDirection);
	//移動距離
	lRayDirection -= *mFramePosition;
	pNormal.z *= 1;
	pOutSlipVector = lRayDirection - ((D3DXVec3Dot(&pNormal, &lRayDirection)) / (powf(D3DXVec3Length(&pNormal), 2.0f)))*pNormal;

	pOutSlipVector *= -1;
	return;
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

	float fAB;
	float fBC;
	float fCA;

	//法線
	DXVector3 vNormal;
	//3頂点から平面方程式を求める
	D3DXPLANE pln;
	D3DXPlaneFromPoints(&pln, &pvA, &pvB, &pvC);
	vNormal.x = pln.a;
	vNormal.y = pln.b;
	vNormal.z = pln.c;
	vNormal.Normalize();

	//各頂点から交点Iに向かうベクトルをvVとする
	DXVector3 vV;
	//辺ABベクトル（頂点Bベクトルー頂点Aベクトル）と、頂点から交点Iへ向かうベクトルの外積を求める
	vV = pvI - pvA;
	D3DXVec3Cross(&vCrossAB, &vAB, &vV);
	vV = pvI - pvB;
	D3DXVec3Cross(&vCrossBC, &vBC, &vV);

	vV = pvI - pvC;
	D3DXVec3Cross(&vCrossCA, &vCA, &vV);
	//それぞれの外積ベクトルとの内積を求める
	fAB = D3DXVec3Dot(&vNormal, &vCrossAB);
	fBC = D3DXVec3Dot(&vNormal, &vCrossBC);
	fCA = D3DXVec3Dot(&vNormal, &vCrossCA);

	//どれか一つでも負数であれば、交点はポリゴンの外
	if (
		fAB >= 0.0f &&
		fBC >= 0.0f&&
		fCA >= 0.0f
		) {
		//交点は面の内側
		return true;
	}
	//交点は面の外
	return false;
}
