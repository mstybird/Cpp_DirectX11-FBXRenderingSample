#include "MSCollisionSphere.h"
#include"DXVector3.h"
#include"MSFbxManager.h"
#include"DX11FbxResource.h"
#include"DXVector3.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"DXCamera.h"
#include"DX11RenderResource.h"
#include<array>
#include<vector>
#include<map>



MSCollisionSphere::MSCollisionSphere() :
	mRadius{ 0 },
	mCenter{ std::make_unique<DXVector3>() }
{
}

void MSCollisionSphere::CreateCollision(const FBXModelData&pMesh, const DXMatrix&pGlobalPosition)
{
	D3DXComputeBoundingSphere((const D3DXVECTOR3*)pMesh.Data.data(), pMesh.Data.size(), sizeof(FbxVertex), mCenter.get(), &mRadius);
	//����g�k��
	DXVector3 lScale, lTrans;
	pGlobalPosition.GetS(lScale);
	pGlobalPosition.GetT(lTrans);
	mRadius *= lScale.GetMedian();
	*mCenter += lTrans;
	mGlobalPosition = std::make_unique<DXMatrix>(pGlobalPosition);
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