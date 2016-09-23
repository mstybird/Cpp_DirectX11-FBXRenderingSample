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
	DX11RenderResource & pTargetResource, 
	DXProjection& pProjection)
{
	D3DXPLANE VFLeftPlane;
	D3DXPLANE VFRightPlane;
	D3DXPLANE VFTopPlane;
	D3DXPLANE VFBottomPlane;
	DXVector3 lCameraPos;
	DXVector3 lTargetPos;
	DXVector3 lTargetScaleVector;
	float lTargetScale;
	//�^�[�Q�b�g�̊g�嗦�𒲐�����K�v������̂ŁA�ʒu�Ɗg�嗦�Ö@�擾�ł���悤�ɂ���
	DXMatrix& lTargetMatrix = *pTargetResource.GetWorld().lock()->GetMatrix().lock();
	//float Radius;
	std::weak_ptr<std::vector<std::vector<MSCollisionSphere>>> lCollisions;
	pTargetResource.GetCollisionSphere(lCollisions);
	if (lCollisions.expired()) {
		assert(0);
	}

	pCameraResource.GetWorld().lock()->GetMatrix().lock()->GetT(lCameraPos);
	//�ʒu���擾
	lTargetMatrix.GetT(lTargetPos);
	//�g�嗦���擾
	lTargetMatrix.GetS(lTargetScaleVector);
	
	//��ԑ傫�Ȋg�嗦�����E���̃T�C�Y�Ƃ��č̗p����
	lTargetScale = lTargetScaleVector.x;
	if(lTargetScale < lTargetScaleVector.y) {
		lTargetScale = lTargetScaleVector.y;
	}
	if (lTargetScale < lTargetScaleVector.z) {
		lTargetScale = lTargetScaleVector.z;
	}

	pTargetResource.GetWorld().lock()->GetMatrix().lock()->GetT(lTargetPos);
	//���[���h��Ԃ���r���[��Ԃɕϊ�
	DXCamera c;
	c.SetCamera(pCameraResource.GetWorld(), { 0,0,-1 });
	D3DXVec3TransformCoord(&lTargetPos, &lTargetPos, c.GetMatrix().lock().get());

	float& lFarClip = pProjection.mFar;
	float& lNearClip = pProjection.mNear;
	float& lAspect = pProjection.mAspect;
	float& lAngle = pProjection.mAngle;

	//���E�A�㉺�̕��ʂ��v�Z
	{
		D3DXVECTOR3 p1, p2, p3;
		//����
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -lFarClip * ((FLOAT)tan(lAngle / 2)*lAspect);
		p2.y = -lFarClip * (FLOAT)tan(lAngle / 2);
		p2.z = lFarClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFLeftPlane, &p1, &p2, &p3);
		//�E��
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = lFarClip * ((FLOAT)tan(lAngle / 2)*lAspect);
		p2.y = lFarClip * (FLOAT)tan(lAngle / 2);
		p2.z = lFarClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFRightPlane, &p1, &p2, &p3);
		//���
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -lFarClip * ((FLOAT)tan(lAngle / 2)*lAspect);
		p2.y = lFarClip * (FLOAT)tan(lAngle / 2);
		p2.z = lFarClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFTopPlane, &p1, &p2, &p3);
		//����
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = lFarClip * ((FLOAT)tan(lAngle / 2)*lAspect);
		p2.y = -lFarClip * (FLOAT)tan(lAngle / 2);
		p2.z = lFarClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFBottomPlane, &p1, &p2, &p3);
	}


	for (const auto& lCollisionList : *lCollisions.lock()) {
		for (const auto& lCollision : lCollisionList) {
			float Radius = lCollision.GetRadius()*lTargetScale;

			DXVector3 Pos;
			lCollision.GetCenterPosition(Pos);
			Pos += lTargetPos;
			//6�̕��ʂƃW�I���g�����E�����`�F�b�N
			{
				//�j�A�N���b�v�ʂɂ���
				if ((Pos.z + Radius) < lNearClip)
				{
					return false;
				}
				//�t�@�[�N���b�v�ʂɂ���
				if ((Pos.z - Radius) > lFarClip)
				{
					return false;
				}
				//���N���b�v�ʂɂ���
				float Distance = (Pos.x * VFLeftPlane.a) + (Pos.z * VFLeftPlane.c);
				if (Distance>Radius)
				{
					return false;
				}
				//�E�N���b�v�ʂɂ���
				Distance = (Pos.x * VFRightPlane.a) + (Pos.z * VFRightPlane.c);
				if (Distance>Radius)
				{
					return false;
				}
				//��N���b�v�ʂɂ���
				Distance = (Pos.y * VFTopPlane.b) + (Pos.z * VFTopPlane.c);
				if (Distance>Radius)
				{
					return false;
				}
				//���N���b�v�ʂɂ���
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
