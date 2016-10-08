#include "MSCullingFrustum.h"
#include"DX11RenderResource.h"
#include"MSFbxManager.h"
#include"DXVector3.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"DXCamera.h"
#include"DXProjection.h"
#include<d3dx9.h>
#include<memory>
#include<cassert>
#include"MSCollisionSphere.h"
bool MSCullingFrustum::IsCullingWorld(
	DX11RenderResource & pCameraResource, 
	DX11RenderResource & pTargetResource)
{
	D3DXPLANE VFLeftPlane;
	D3DXPLANE VFRightPlane;
	D3DXPLANE VFTopPlane;
	D3DXPLANE VFBottomPlane;
	DXVector3 lCameraPos;
	DXVector3 lTargetPos;
	DXVector3 lTargetScaleVector;
	float lTargetScale;
	//ターゲットの拡大率を調整する必要があるので、位置と拡大率療法取得できるようにする
	DXMatrix& lTargetMatrix = *pTargetResource.GetWorld().lock()->GetMatrix().lock();
	//float Radius;
	auto& lCollisions = pTargetResource.mMesh.mCollisionSphere;

	pCameraResource.GetWorld().lock()->GetMatrix().lock()->GetT(lCameraPos);
	//位置を取得
	lTargetMatrix.GetT(lTargetPos);
	//拡大率を取得
	lTargetMatrix.GetS(lTargetScaleVector);
	
	//一番大きな拡大率を境界球のサイズとして採用する
	lTargetScale = lTargetScaleVector.x;
	if(lTargetScale < lTargetScaleVector.y) {
		lTargetScale = lTargetScaleVector.y;
	}
	if (lTargetScale < lTargetScaleVector.z) {
		lTargetScale = lTargetScaleVector.z;
	}

	pTargetResource.GetWorld().lock()->GetMatrix().lock()->GetT(lTargetPos);
	//ワールド空間からビュー空間に変換
	DXCamera c;
	c.SetCamera(pCameraResource.GetWorld(), { 0,0,-1 });
	D3DXVec3TransformCoord(&lTargetPos, &lTargetPos, c.GetMatrix().lock().get());

	DXProjection& lEyeProjection = *pCameraResource.GetProjection().lock();
	float& lFarClip = lEyeProjection.mFar;
	float& lNearClip = lEyeProjection.mNear;
	float& lAspect = lEyeProjection.mAspect;
	float& lAngle = lEyeProjection.mAngle;

	//左右、上下の平面を計算
	{
		D3DXVECTOR3 p1, p2, p3;
		//左面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -lFarClip * ((FLOAT)tan(lAngle / 2)*lAspect);
		p2.y = -lFarClip * (FLOAT)tan(lAngle / 2);
		p2.z = lFarClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFLeftPlane, &p1, &p2, &p3);
		//右面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = lFarClip * ((FLOAT)tan(lAngle / 2)*lAspect);
		p2.y = lFarClip * (FLOAT)tan(lAngle / 2);
		p2.z = lFarClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFRightPlane, &p1, &p2, &p3);
		//上面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -lFarClip * ((FLOAT)tan(lAngle / 2)*lAspect);
		p2.y = lFarClip * (FLOAT)tan(lAngle / 2);
		p2.z = lFarClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFTopPlane, &p1, &p2, &p3);
		//下面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = lFarClip * ((FLOAT)tan(lAngle / 2)*lAspect);
		p2.y = -lFarClip * (FLOAT)tan(lAngle / 2);
		p2.z = lFarClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFBottomPlane, &p1, &p2, &p3);
	}


	for (const auto& lCollisionList : lCollisions) {
		for (const auto& lCollision : lCollisionList) {
			float Radius = lCollision.GetRadius()*lTargetScale;

			DXVector3 Pos;
			lCollision.GetCenterPosition(Pos);
			Pos += lTargetPos;
			//6つの平面とジオメトリ境界球をチェック
			{
				//ニアクリップ面について
				if ((Pos.z + Radius) < lNearClip)
				{
					return false;
				}
				//ファークリップ面について
				if ((Pos.z - Radius) > lFarClip)
				{
					return false;
				}
				//左クリップ面について
				float Distance = (Pos.x * VFLeftPlane.a) + (Pos.z * VFLeftPlane.c);
				if (Distance>Radius)
				{
					return false;
				}
				//右クリップ面について
				Distance = (Pos.x * VFRightPlane.a) + (Pos.z * VFRightPlane.c);
				if (Distance>Radius)
				{
					return false;
				}
				//上クリップ面について
				Distance = (Pos.y * VFTopPlane.b) + (Pos.z * VFTopPlane.c);
				if (Distance>Radius)
				{
					return false;
				}
				//下クリップ面について
				Distance = (Pos.y * VFBottomPlane.b) + (Pos.z * VFBottomPlane.c);
				if (Distance>Radius)
				{
					return false;
				}
			}
		}
	}



	return true;
}
