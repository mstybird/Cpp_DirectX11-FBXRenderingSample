#include "MSCollision.h"
#include"DXVector3.h"
#include"MSFbxManager.h"
#include"DX11FbxResource.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"DXCamera.h"
#include"DX11RenderResource.h"
#include<array>
MSCollisionSphere::MSCollisionSphere():
	mRadius{0},
	mCenter{std::make_unique<DXVector3>()}
{
}

void MSCollisionSphere::CreateCollision(const FBXModelData&pMesh, const DXMatrix&pGlobalPosition)
{
	D3DXComputeBoundingSphere((const D3DXVECTOR3*)pMesh.Data.data(), pMesh.Data.size(), sizeof(FbxVertex), mCenter.get(), &mRadius);
	//����g�k��
	DXVector3 lScale,lTrans;
	pGlobalPosition.GetS(lScale);
	pGlobalPosition.GetT(lTrans);
	mRadius *= lScale.GetMedian();
	*mCenter += lTrans;
	mGlobalPosition =std::make_unique<DXMatrix>(pGlobalPosition);
}

bool MSCollisionSphere::Collision(MSCollisionSphere&pCollision1,
	const DXWorld&pWorldMatrix1,
	MSCollisionSphere&pCollision2,
	const DXWorld&pWorldMatrix2)
{
	//���\�[�X��̈ʒu
	DXVector3& lResPosition1 = *pWorldMatrix1.mPosition;
	DXVector3& lResPosition2 = *pWorldMatrix2.mPosition;

	//FBX��̃O���[�o���|�W�V����
	DXVector3& lFbxPosition1 = *pCollision1.mCenter;
	DXVector3& lFbxPosition2 = *pCollision2.mCenter;

	//���݂̃��\�[�X�̍s����g���Ĕ���ʒu�̏C�������Ă���
	const DXVector3 lDistance = (lResPosition1 + lFbxPosition1) - (lResPosition2 + lFbxPosition2);

	float lLength = lDistance.GetDistance();

	//���\�[�X��̊g�听�����擾
	DXVector3& lResScaling1 = *pWorldMatrix1.mScale;
	DXVector3& lResScaling2 = *pWorldMatrix2.mScale;
	//FBX��̊g�听���𔽉f�������a
	float& lFbxScaling1 = pCollision1.mRadius;
	float& lFbxScaling2 = pCollision2.mRadius;

	float lRadius1 = lResScaling1.GetMedian()*lFbxScaling1;
	float lRadius2 = lResScaling2.GetMedian()*lFbxScaling2;

	return(lLength <= lRadius1 + lRadius2);
}

MSCollisionBase::MSCollisionBase():
	mGlobalPosition{std::make_unique<DXMatrix>()}
{
}

MSCollisionBase::MSCollisionBase(const MSCollisionBase & _co)
{
	mGlobalPosition = std::make_unique<DXMatrix>(*this->mGlobalPosition);
}

MSCollisionBase::~MSCollisionBase()
{
}

MSCollisionRay::MSCollisionRay():
	mRayDirection{std::make_shared<DXVector3>()}
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

bool MSCollisionRay::Collision(const MSCollisionRay & pRay, DX11RenderResource&pRayPosition, DX11RenderResource & pResource, MSFbxManager & pFbxManager)
{
	//1�̃|���S����3�̒��_
	std::array<FbxVertex*, 3>lPolygon;
	//���\�[�X�̃��[���h���W
	DXVector3 lPositionOffset;
	DXVector3 lPosition[3];
	pResource.GetWorld().lock()->GetMatrix().lock()->GetT(lPositionOffset);
	//�|���S���擾


	float lDistance;	

	


	auto lmeshList = pFbxManager.GetMeshData();
	DXMatrix lResourceMatrix = *pResource.GetWorld().lock()->GetMatrix().lock();
	for (auto&lMesh : *lmeshList) {

		//�ϊ��s��̍���
		DXMatrix lGlobalMatrix{ *lMesh->mWorld.get() };
		lGlobalMatrix = lGlobalMatrix * lResourceMatrix;
		for (auto&lSubMesh : lMesh->subMesh) {
			for (int i = 0; i < lSubMesh->PolygonCount; i++) {
				//������s���|���S�����擾
				lSubMesh->GetPolygon(lPolygon, i);

				//���\�[�X��̈ʒu��^�����s����쐬����
				DXMatrix lPositionMatrix[3];

				for (int i = 0; i < 3; ++i) {
					lPositionMatrix[i].Translation(lPolygon[i]->Pos);
					lPositionMatrix[i] *= lGlobalMatrix;
					lPositionMatrix[i].GetT(lPosition[i]);
				}
				//���C����
				DXVector3 lRayPos;
				pRayPosition.GetCamera().lock()->GetEyeT(lRayPos);
//				pRayPosition.GetWorld().lock()->GetMatrix().lock()->GetT(lRayPos);
				if(D3DXIntersectTri(
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
