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
	//1�̃|���S����3�̒��_
	std::array<FbxVertex*, 3>lPolygon;
	//���\�[�X�̃��[���h���W
	DXVector3 lPosition[3];
	//�|���S���擾


	float lDistance;

	


	auto lmeshList = pFbxObject.GetCurrentMeshData();
	DXMatrix lResourceMatrix = *pResource.GetWorld().lock()->GetMatrix().lock();
	for (auto&lMesh : *lmeshList) {

		//�ϊ��s��̍���
		DXMatrix lGlobalMatrix{ *lMesh->mWorld.get() };
		lGlobalMatrix = lGlobalMatrix * lResourceMatrix;
		for (auto&lSubMesh : lMesh->subMesh) {
			for (uint32_t i = 0; i < lSubMesh->PolygonCount; i++) {
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
