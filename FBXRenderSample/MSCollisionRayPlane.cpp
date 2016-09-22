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
	//���C�̎n�_�ƏI�_
	DXVector3 vP[2];

	//���C�̎n�_���擾���A�I�_���v�Z����
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



		//������������
		pOutDistance = FLT_MAX;
		DXVector3 lPosition[3];		//���C����g���ŏI�I��3���_
		for (auto&lMesh : *lMeshList) {
			//�ϊ��s��̍���
			//���b�V���ɒ�`����Ă���s����擾
			DXMatrix lGlobalMatrix{ *lMesh->mWorld.get() };
			//����
			lGlobalMatrix = lGlobalMatrix * lResourceMatrix;
			//�T�u���b�V���P�ʂŃ��[�v
			for (auto&lSubMesh : lMesh->subMesh) {
				//�|���S���P�ʂŃ��[�v
				for (int i = 0; i < lSubMesh->PolygonCount; i++) {
					FbxPtrPolygon lPtrPolygon;
					//���b�V�����\�������̃|���S�����擾����
					lSubMesh->GetPolygon(lPtrPolygon, i);

					//�s����g���Ē��_�ʒu�̏C��
					DXMatrix lPositionMatrix[3];
					for (int i = 0; i < 3; ++i) {
						lPositionMatrix[i].Translation(lPtrPolygon[i]->Pos);
						lPositionMatrix[i] *= lGlobalMatrix;
						lPositionMatrix[i].GetT(lPosition[i]);
					}


					//����
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
						// t ��0����1�̊Ԃł���Ȃ�������Ă��邱�ƂɂȂ�
						if (t >= 0 && t <= 1.0f) {

							//�������Ă����ꍇ�A��_���W�𓾂�
							DXVector3 lCrossPoint;
							//���ꂼ��̃x�N�g���̒����ɔ}��ϐ��������āA�����_���v�Z����
							lCrossPoint.x = t*vP[0].x + (1 - t)*vP[1].x;
							lCrossPoint.y = t*vP[0].y + (1 - t)*vP[1].y;
							lCrossPoint.z = t*vP[0].z + (1 - t)*vP[1].z;

							if (IsInside(
								lCrossPoint,
								lPosition[0],
								lPosition[1],
								lPosition[2]
							)) {
								//���C���ˈʒu�����_���������x�N�g���̋������A��_�Ƃ̋���
								float lTmpDistance = (vP[0] - lCrossPoint).GetDistance();
								//���ɏՓ˂����|���S�����߂���΍X�V
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
	//���C�̕������v�Z����
	DXVector3 lRayDirection;
	pRayPosition.GetWorld().lock()->GetMatrix().lock()->GetT(lRayDirection);
	lRayDirection -= *mFramePosition;
	//�������m��
	lRayDirection.Normalize();
	//�����������ă��C�̒������v�Z����
	lRayDirection *= pRayLength;
	//���̃��C���g���Ĕ���
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
	//�ړ��x�N�g��(����x�N�g���̌v�Z
	//���C�̕������v�Z����
	DXVector3 lRayDirection;
	pUpdPosition.GetWorld().lock()->GetMatrix().lock()->GetT(lRayDirection);
	//�ړ�����
	lRayDirection -= *mFramePosition;
	pNormal.z *= 1;
	pOutSlipVector = lRayDirection - ((D3DXVec3Dot(&pNormal, &lRayDirection)) / (powf(D3DXVec3Length(&pNormal), 2.0f)))*pNormal;

	pOutSlipVector *= -1;
	return;
}

bool MSCollisionRayPlane::IsInside( DXVector3 & pvI,  DXVector3 & pvA,  DXVector3 & pvB,  DXVector3 & pvC)
{
	//�Ӄx�N�g��(�ӂ̒���)
	DXVector3 vAB;
	DXVector3 vBC;
	DXVector3 vCA;
	vAB = pvB - pvA;
	vBC = pvC - pvB;
	vCA = pvA - pvC;

	//�Ӄx�N�g���ƒ��_�����_�֌������x�N�g���Ƃ̂��ꂼ��̊O�ϗp
	DXVector3 vCrossAB;
	DXVector3 vCrossBC;
	DXVector3 vCrossCA;

	float fAB;
	float fBC;
	float fCA;

	//�@��
	DXVector3 vNormal;
	//3���_���畽�ʕ����������߂�
	D3DXPLANE pln;
	D3DXPlaneFromPoints(&pln, &pvA, &pvB, &pvC);
	vNormal.x = pln.a;
	vNormal.y = pln.b;
	vNormal.z = pln.c;
	vNormal.Normalize();

	//�e���_�����_I�Ɍ������x�N�g����vV�Ƃ���
	DXVector3 vV;
	//��AB�x�N�g���i���_B�x�N�g���[���_A�x�N�g���j�ƁA���_�����_I�֌������x�N�g���̊O�ς����߂�
	vV = pvI - pvA;
	D3DXVec3Cross(&vCrossAB, &vAB, &vV);
	vV = pvI - pvB;
	D3DXVec3Cross(&vCrossBC, &vBC, &vV);

	vV = pvI - pvC;
	D3DXVec3Cross(&vCrossCA, &vCA, &vV);
	//���ꂼ��̊O�σx�N�g���Ƃ̓��ς����߂�
	fAB = D3DXVec3Dot(&vNormal, &vCrossAB);
	fBC = D3DXVec3Dot(&vNormal, &vCrossBC);
	fCA = D3DXVec3Dot(&vNormal, &vCrossCA);

	//�ǂꂩ��ł������ł���΁A��_�̓|���S���̊O
	if (
		fAB >= 0.0f &&
		fBC >= 0.0f&&
		fCA >= 0.0f
		) {
		//��_�͖ʂ̓���
		return true;
	}
	//��_�͖ʂ̊O
	return false;
}
