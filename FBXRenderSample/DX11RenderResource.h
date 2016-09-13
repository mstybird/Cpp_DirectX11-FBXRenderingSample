#pragma once
#include<d3dx9.h>
/*
	����
	Sprite2D
	Sprite3D
	SpriteScreen
	C++11�ڍs


*/
#include<memory>
#include<vector>
class DXDisplay;
class DXWorld;
class DXCamera;
class DXProjection;
class DXMatrix;
class MSCollisionSphere;
class DX11RenderResource {
public:

	DX11RenderResource();
	~DX11RenderResource();
	//�`��ɕK�v�ȃr���[�s��Ǝˉe�s�񂪕K�v�ȏꍇ�K���Ă�
	void InitRenderMatrix();
	std::weak_ptr<D3DXMATRIX>GetMatrixWorld();
	std::weak_ptr<D3DXMATRIX>GetMatrixView();
	std::weak_ptr<D3DXMATRIX>GetMatrixProjection();
	std::unique_ptr<D3DXMATRIX> GetMatrixWVP(const std::weak_ptr<DXDisplay>pDisplay);
	std::unique_ptr<D3DXMATRIX> GetMatrixWVP(const std::weak_ptr<D3DXMATRIX>pAddWorldMatrix, const std::weak_ptr<DXDisplay>pDisplay);

	std::weak_ptr<DXWorld>GetWorld();
	std::weak_ptr<DXCamera>GetCamera();
	std::weak_ptr<DXProjection>GetProjection();

private:
	//���[���h�s��
	std::shared_ptr<DXWorld>mWorld;
	//�r���[�s��
	std::shared_ptr<DXCamera>mView;
	//�ˉe�s��
	std::shared_ptr<DXProjection>mProj;
	//���E���R���W�����Q��
	std::weak_ptr<std::vector<std::vector<MSCollisionSphere>>>mCollisions;
};