#include "MSCollision.h"
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
using std::pair;
using std::vector;
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
	DXVector3 lPosition[3];
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

MSCollisionRayPicking::MSCollisionRayPicking():
	mSlipFlag{false},
	mPosBefore{std::make_unique<DXVector3>()}
{
}

void MSCollisionRayPicking::SetFramePosition(DX11RenderResource & pRayPosion)
{
	//���݂̍��W�����擾
	 pRayPosion.GetWorld().lock()->GetMatrix().lock()->GetT(*mPosBefore);
}

void MSCollisionRayPicking::SetSlipFlag(bool pSlipFlag)
{
	mSlipFlag = pSlipFlag;
}

bool MSCollisionRayPicking::GetSlipFlag()
{
	return mSlipFlag;
}

bool MSCollisionRayPicking::Collision(DXVector3&pResultPosition,DX11RenderResource&pRayPosition, DX11RenderResource & pRayTarget, MSFbxManager & pFbxTarget)
{
	using FbxPolygon = std::array<FbxVertex, 3>;
	using FbxPtrPolygon = std::array<FbxVertex*, 3>;

	struct T_MeshPolygon {
		std::weak_ptr<FBXModelData> mesh;	//�����������b�V��
		FbxPolygon polygon;					//�q�b�g�����|���S��
		DXMatrix globalMatrix;				//�q�b�g�������b�V���ɓK�p����Ă���s��
	};

	//�q�b�g�����|���S�����܂ރ��b�V�������i�[����
	pair<float, T_MeshPolygon>lHitPolygon{ FLT_MAX,T_MeshPolygon() };
	bool lHit{ false };			//���b�V���ɓ��������ꍇ��true
	float lDistance{ 0.0f };	//���������ꍇ�̋���
	DXVector3 lRayAfterPosition{};	//�X�V��̈ʒu���擾����

	//���C����
	{
		FbxPolygon lPolygon;
		//�����������̏����󂯎��ϐ�
		DXVector3 lDirection;//�ϊ���̈ʒu�A�������i�[����ϐ�
		DXMatrix invMat;		//�t�s����i�[����ϐ�
		//���\�[�X�̃��[���h���W
		DXVector3 lPosition[3];		//���C����g���ŏI�I��3���_

		//�^�[�Q�b�g���\�[�X��̍s����擾
		DXMatrix&lResourceMatrix = *pRayTarget.GetWorld().lock()->GetMatrix().lock();
		
		//�X�V��̃��\�[�X����ʒu���擾
		pRayPosition.GetWorld().lock()->GetMatrix().lock()->GetT(lRayAfterPosition);


		//���b�V�����������ƂɃ\�[�g
		auto lMeshList = pFbxTarget.GetMeshData();
		//�ړ������Ƀ|���S�������邩�`�F�b�N
		//�����Ń��C���q�b�g�����ԋ߂��|���S�����擾����

		DXVector3 lTmpPosBefore= *mPosBefore;
		for (auto&lMesh : *lMeshList) {

			*mPosBefore = lTmpPosBefore;
			//�ϊ��s��̍���
			//���b�V���ɒ�`����Ă���s����擾
			DXMatrix lGlobalMatrix{ *lMesh->mWorld.get() };
			//����
			lGlobalMatrix = lGlobalMatrix * lResourceMatrix;
			//�t�s��̎擾
			//D3DXMatrixInverse(&invMat, nullptr, &lGlobalMatrix);
			//���C�n�_����ϊ�
			//D3DXVec3TransformCoord(&*mPosBefore, &*mPosBefore, &invMat);
			//D3DXVec3TransformCoord(&lRayAfterPosition, &lRayAfterPosition, &invMat);
			//���C�������v�Z
			D3DXVec3Normalize(&lDirection, &(lRayAfterPosition-*mPosBefore));

			for (auto&lSubMesh : lMesh->subMesh) {
				for (int i = 0; i < lSubMesh->PolygonCount; i++) {
					//������s���|���S�����擾
					FbxPtrPolygon lPtrPolygon;
					lSubMesh->GetPolygon(lPtrPolygon, i);

					//�s����g���Ē��_�ʒu�̏C��
					DXMatrix lPositionMatrix[3];
					for (int i = 0; i < 3; ++i) {
						lPositionMatrix[i].Translation(lPtrPolygon[i]->Pos);
						lPositionMatrix[i] *= lGlobalMatrix;
						lPositionMatrix[i].GetT(lPosition[i]);
					}

					
				


					//�Փ˔�����s��
					//���C����
					if (D3DXIntersectTri(
						&lPosition[0],
						&lPosition[1],
						&lPosition[2],
						&*mPosBefore,
						&lDirection,
						NULL,
						NULL,
						&lDistance
					)) {
						lHit = true;

						//���݂̃|���S����苗�����߂�΂��̃|���S���ōX�V
						if (lHitPolygon.first > lDistance) {
							lHitPolygon.first = lDistance;
							lHitPolygon.second.mesh = lSubMesh;
							lHitPolygon.second.globalMatrix = lGlobalMatrix;
							lHitPolygon.second.polygon[0] = *lPtrPolygon[0];
							lHitPolygon.second.polygon[1] = *lPtrPolygon[1];
							lHitPolygon.second.polygon[2] = *lPtrPolygon[2];

							//���_���͍s��ɂ���ĕω����Ă��邽�ߍX�V����
							lHitPolygon.second.polygon[0].Pos = lPosition[0];
							lHitPolygon.second.polygon[1].Pos = lPosition[1];
							lHitPolygon.second.polygon[2].Pos = lPosition[2];

							//�@�����͍s��ɂ���ĕω����Ă��邽�ߍX�V����
							DXMatrix lNormalMatrix[3];
							for (int i = 0; i < 3; ++i) {
								lNormalMatrix[i] *= lGlobalMatrix;
								D3DXVec3TransformNormal(&lHitPolygon.second.polygon[i].Normal, &lHitPolygon.second.polygon[i].Normal, &lGlobalMatrix);
								lHitPolygon.second.polygon[i].Normal.Normalize();
							}

						}
					}
				}
			}


		}

	}
	//���C���q�b�g�����ꍇ�A���ꂪ���ۂɏՓ˂��������m����

	//���C���q�b�g���Ȃ������ꍇ�A�����ŏ����I���B
	//false��Ԃ�
	if (!lHit)return false;
//	printf("RayHit!\n");
	DXVector3 polyNormal;	//�|���S���̖@��

	//�|���S���̌������v�Z
	{
		enum class E_POS {
			NONE,X,Y,Z
		};

		//�|���S������e�@�������߂�
		auto fNormalMul = [&](E_POS pType)->float {
			float lResult;
			switch (pType)
			{
			case E_POS::X:
				lResult = lHitPolygon.second.polygon[0].Normal.x + lHitPolygon.second.polygon[1].Normal.x + lHitPolygon.second.polygon[2].Normal.x;
				break;
			case E_POS::Y:
				lResult = lHitPolygon.second.polygon[0].Normal.y + lHitPolygon.second.polygon[1].Normal.y + lHitPolygon.second.polygon[2].Normal.y;
				break;
			case E_POS::Z:
				lResult = lHitPolygon.second.polygon[0].Normal.z + lHitPolygon.second.polygon[1].Normal.z + lHitPolygon.second.polygon[2].Normal.z;
				break;
			default:
				break;
			}
			return lResult / 3.0f;
		};

		polyNormal.x = fNormalMul(E_POS::X);
		polyNormal.y = fNormalMul(E_POS::Y);
		polyNormal.z = fNormalMul(E_POS::Z);

	}

	D3DXPLANE lPlane;		//���ʏ����p
	//3�_���g���Ė������ʂ̒�`
	D3DXPlaneFromPoints(
		&lPlane,
		&lHitPolygon.second.polygon[0].Pos,
		&lHitPolygon.second.polygon[1].Pos,
		&lHitPolygon.second.polygon[2].Pos
	);

	//�ړ���̍��W���q�b�g������ԋ߂��|���S�����܂����ł��邩����
	{
		float lAns[2];

		auto fPlaneCalc = [&](const DXVector3&pPosition)->float {
			float lResult;
			lResult= (lPlane.a * pPosition.x) + (lPlane.b * pPosition.y) + (lPlane.c * pPosition.z) + lPlane.d;
			return lResult;
		};

		lAns[0] = fPlaneCalc(lRayAfterPosition);
		lAns[1] = fPlaneCalc(*mPosBefore);
		printf("%2.2f:%2.2f\n", lAns[0], lAns[1]);
		//�Q�̐��������΂ł���΁A�|���S�����܂����ł���
		if (lAns[0] * lAns[1] < 0.0f) {
			//����ꍇ�̏���
			if (mSlipFlag == true) {
				//��������̍��W������ꎞ�ϐ�
				DXVector3 lAfterPos;

				//��ԋ߂��|���S���̋���
				float lNearDistance = FLT_MAX;
				//���C����
				for (int i = 0; i < lHitPolygon.second.mesh.lock()->PolygonCount; ++i) {
					//������s���|���S���̎擾
					FbxPtrPolygon lPtrPolygon;
					lHitPolygon.second.mesh.lock()->GetPolygon(lPtrPolygon, i);

					//�s����g���Ē��_�ʒu�̏C��
					DXMatrix lPositionMatrix[3];
					DXVector3 lPosition[3];
					for (int i = 0; i < 3; ++i) {
						lPositionMatrix[i].Translation(lPtrPolygon[i]->Pos);
						lPositionMatrix[i] *= lHitPolygon.second.globalMatrix;
						lPositionMatrix[i].GetT(lPosition[i]);
					}
					//�ړ���̍��W���烌�C���΂��ă|���S���Ƃ̌�_���v�Z
					if (D3DXIntersectTri(
						&lPosition[0],
						&lPosition[1],
						&lPosition[2],
						&lRayAfterPosition,
						&polyNormal,
						nullptr,
						nullptr,
						&lDistance
					)) {
						//���݃q�b�g���Ă���|���S�����߂��|���S���̏Փ˂����ꍇ
						if (lNearDistance > lDistance) {
							lNearDistance = lDistance;
						}
					}

				}

				if (lNearDistance < FLT_MAX) {
					//���������ꍇ�A�����̍��W���v�Z����(�������߂ɉ����߂�
					lAfterPos = lRayAfterPosition + polyNormal*(lDistance + 0.05f);
					//���ׂĂ̏������I���΃Z�b�g����
					pResultPosition = lAfterPos;
				}
				else {
					return false;
				}

			}
			//����Ȃ��ꍇ�̏���
			else {
			
			}
			//���������̂�true��Ԃ�
			return true;
		}

		//2�Ƃ����������ꍇ�̓|���S���ɂ߂荞��ł���̂ŁA���Ε����̒��߃|���S���܂ňړ�
		if (lAns[0] < 0.0f&&lAns[1] < 0.0f) {
			//����ꍇ�̏���
			if (mSlipFlag == true) {
				//��������̍��W������ꎞ�ϐ�
				DXVector3 lAfterPos;

				//��ԋ߂��|���S���̋���
				float lNearDistance = FLT_MAX;
				//���C����
				//���Ε����Ƀ��C���΂�
				//�|���S���ɂ߂荞��ł����Ԃ��O������Ȃ̂ŁA
				//���Ε����Ƀ��C���΂��΁A�|���S���Ƀq�b�g����
				DXVector3 lDirection;
				D3DXVec3Normalize(&lDirection, &(*mPosBefore-lRayAfterPosition));
				for (int i = 0; i < lHitPolygon.second.mesh.lock()->PolygonCount; ++i) {
					//������s���|���S���̎擾
					FbxPtrPolygon lPtrPolygon;
					lHitPolygon.second.mesh.lock()->GetPolygon(lPtrPolygon, i);

					//�s����g���Ē��_�ʒu�̏C��
					DXMatrix lPositionMatrix[3];
					DXVector3 lPosition[3];
					for (int i = 0; i < 3; ++i) {
						lPositionMatrix[i].Translation(lPtrPolygon[i]->Pos);
						lPositionMatrix[i] *= lHitPolygon.second.globalMatrix;
						lPositionMatrix[i].GetT(lPosition[i]);
					}
					//�ړ���̍��W���烌�C���΂��ă|���S���Ƃ̌�_���v�Z
					if (D3DXIntersectTri(
						&lPosition[0],
						&lPosition[1],
						&lPosition[2],
						&lRayAfterPosition,
						&lDirection,
						nullptr,
						nullptr,
						&lDistance
					)) {
						//���݃q�b�g���Ă���|���S�����߂��|���S���̏Փ˂����ꍇ�A�������X�V
						//���̂悤�ɂ��Ȃ��ƕ��G�Ȍ`�������|���S���̏ꍇ�A
						//����ʏꏊ�ɍs���Ă��܂������ꂪ����
						if (lNearDistance > lDistance) {
							lNearDistance = lDistance;
						}
					}

				}

				if (lNearDistance < FLT_MAX) {
					//���������ꍇ�A�����̍��W���v�Z����(�������߂ɉ����߂�
					lAfterPos = lRayAfterPosition + lDirection*(lDistance + 0.05f);
					//���ׂĂ̏������I���΃Z�b�g����
					pResultPosition = lAfterPos;
					return true;
				}
				else {
					return false;
				}


			}
		}
		
	}
	

	//���C�͓����������̂́A�Փ˂͂��Ȃ������̂�false��Ԃ�
	return false;
}
