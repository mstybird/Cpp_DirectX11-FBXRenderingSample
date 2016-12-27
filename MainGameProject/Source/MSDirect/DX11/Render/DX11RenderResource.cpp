#include "DX11RenderResource.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"DXCamera.h"
#include"DXProjection.h"
#include"DXDisplay.h"
#include"MSCollisionSphere.h"
DX11RenderResource::DX11RenderResource()
{

}

DX11RenderResource::~DX11RenderResource()
{

}

void DX11RenderResource::InitRenderMatrix()
{
}

D3DXMATRIX* DX11RenderResource::GetMatrixWorld()
{
	return mWorld.GetMatrix();
}

D3DXMATRIX* DX11RenderResource::GetMatrixView()
{
	return mView.GetMatrix();
}

D3DXMATRIX* DX11RenderResource::GetMatrixProjection()
{
	return mProj.GetMatrix();
}

void DX11RenderResource::GetMatrixWVP(D3DXMATRIX* pOut, const DXDisplay& pDisplay)
{
	
	*pOut = *mWorld.GetMatrix() * *pDisplay.GetCamera()->GetMatrix()* *pDisplay.GetProjection()->GetMatrix();

	//std::unique_ptr<D3DXMATRIX> lResult = std::make_unique<D3DXMATRIX>();
	//*lResult=*mWorld->GetMatrix().lock() * *pDisplay.GetCamera().lock()->GetMatrix().lock() * *pDisplay.GetProjection().lock()->GetMatrix().lock();
	return;
}

void DX11RenderResource::GetMatrixWVP(D3DXMATRIX* pOut, const D3DXMATRIX& pAddWorldMatrix, const DXDisplay& pDisplay)
{
	*pOut=pAddWorldMatrix* *mWorld.GetMatrix() * *pDisplay.GetCamera()->GetMatrix()* *pDisplay.GetProjection()->GetMatrix();

	//std::unique_ptr<D3DXMATRIX> lResult = std::make_unique<D3DXMATRIX>();
	//*lResult = pAddWorldMatrix * *mWorld->GetMatrix().lock() *
	//	*pDisplay.GetCamera().lock()->GetMatrix().lock() *
	//	*pDisplay.GetProjection().lock()->GetMatrix().lock();
	return;
}

void DX11RenderResource::SetWorld(const DXWorld & pWorld)
{
	mWorld = pWorld;
}

void DX11RenderResource::SetCamera(const DXCamera & pCamera)
{
	mView = pCamera;
}

void DX11RenderResource::SetProjection(const DXProjection & pProjection)
{

	mProj = pProjection;
}

DXWorld* DX11RenderResource::GetWorld()
{
	return &mWorld;
}

DXCamera* DX11RenderResource::GetCamera()
{
	return &mView;
}

DXProjection* DX11RenderResource::GetProjection()
{
	return &mProj;
}

void DX11RenderResource::SetCollisionSphere(const std::shared_ptr<std::vector<std::vector<MSCollisionSphere>>>&pCollisions)
{
	mCollisions = pCollisions;
}

bool DX11RenderResource::CollisionSphere(std::shared_ptr<DX11RenderResource>&pResource)
{

	for (auto&tX : *this->mCollisions.lock()) {
		for (auto&tCollision : tX) {
			for (auto&pX : *pResource->mCollisions.lock()) {
				for (auto&pCollision : pX) {

					if (MSCollisionSphere::Collision(
						tCollision, *this->GetWorld(),
						pCollision, *pResource->GetWorld()
						)) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

void DX11RenderResource::GetCollisionSphere(std::weak_ptr<std::vector<std::vector<MSCollisionSphere>>> &pOutCollisions)
{
	pOutCollisions = mCollisions;
}

int DX11RenderResource::GetMeshCount()
{
	return mMesh->GetCurrentMeshData()->size();;
}

int DX11RenderResource::GetSubMeshCount(int aMeshIndex)
{
	return mMesh->GetCurrentMeshData()->at(aMeshIndex)->subMesh.size();
}

MSFbxObject * DX11RenderResource::GetMesh()
{
	if (!mMesh)mMesh = std::make_shared<MSFbxObject>();
	return mMesh.get();
}

std::vector<float>* DX11RenderResource::GetTransVector()
{
	return &mTransparents;
}
