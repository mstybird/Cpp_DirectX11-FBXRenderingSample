#include "DX11RenderResource.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"DXCamera.h"
#include"DXProjection.h"
#include"DXDisplay.h"
#include"MSCollisionSphere.h"
DX11RenderResource::DX11RenderResource():
	mWorld{std::make_shared<DXWorld>()}
{

}

DX11RenderResource::~DX11RenderResource()
{

}

void DX11RenderResource::InitRenderMatrix()
{
	mView = std::make_shared<DXCamera>();
	mProj = std::make_shared<DXProjection>();
}

std::weak_ptr<D3DXMATRIX> DX11RenderResource::GetMatrixWorld()
{
	return mWorld->GetMatrix();
}

std::weak_ptr<D3DXMATRIX> DX11RenderResource::GetMatrixView()
{
	return mView->GetMatrix();
}

std::weak_ptr<D3DXMATRIX> DX11RenderResource::GetMatrixProjection()
{
	return mProj->GetMatrix();
}

std::unique_ptr<D3DXMATRIX> DX11RenderResource::GetMatrixWVP(const std::weak_ptr<DXDisplay>pDisplay)
{
	
	std::unique_ptr<D3DXMATRIX> lResult = std::make_unique<D3DXMATRIX>();
	*lResult=*mWorld->GetMatrix().lock() * *pDisplay.lock()->GetCamera().lock()->GetMatrix().lock() * *pDisplay.lock()->GetProjection().lock()->GetMatrix().lock();
	return std::move(lResult);
}

std::unique_ptr<D3DXMATRIX> DX11RenderResource::GetMatrixWVP(const std::weak_ptr<D3DXMATRIX>pAddWorldMatrix, const std::weak_ptr<DXDisplay>pDisplay)
{
	std::unique_ptr<D3DXMATRIX> lResult = std::make_unique<D3DXMATRIX>();
	*lResult = *pAddWorldMatrix.lock() * *mWorld->GetMatrix().lock() *
		*pDisplay.lock()->GetCamera().lock()->GetMatrix().lock() *
		*pDisplay.lock()->GetProjection().lock()->GetMatrix().lock();
	return std::move(lResult);
}

std::weak_ptr<DXWorld> DX11RenderResource::GetWorld()
{
	return mWorld;
}

std::weak_ptr<DXCamera> DX11RenderResource::GetCamera()
{
	return mView;
}

std::weak_ptr<DXProjection> DX11RenderResource::GetProjection()
{
	return mProj;
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
						tCollision, *this->GetWorld().lock(),
						pCollision, *pResource->GetWorld().lock()
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
