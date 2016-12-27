#pragma once
#include<d3dx9.h>
/*
	����
	Sprite2D
	Sprite3D
	SpriteScreen
	C++11�ڍs


*/
#include"MSFbxObject.h"
#include<DXMath.hpp>
#include<memory>
#include<vector>
class DXDisplay;
class DXMatrix;
class MSCollisionSphere;
class DX11RenderResource {
public:

	DX11RenderResource();
	~DX11RenderResource();
	//�`��ɕK�v�ȃr���[�s��Ǝˉe�s�񂪕K�v�ȏꍇ�K���Ă�
	void InitRenderMatrix();
	D3DXMATRIX* GetMatrixWorld();
	D3DXMATRIX* GetMatrixView();
	D3DXMATRIX* GetMatrixProjection();
	void GetMatrixWVP(D3DXMATRIX* pOut, const DXDisplay& pDisplay);
	void GetMatrixWVP(D3DXMATRIX* pOut, const D3DXMATRIX& pAddWorldMatrix, const DXDisplay& pDisplay);


	void SetWorld(const DXWorld&pWorld);
	void SetCamera(const DXCamera&pCamera);
	void SetProjection(const DXProjection&pProjection);
	DXWorld* GetWorld();
	DXCamera* GetCamera();
	DXProjection* GetProjection();

	//���E���̐ݒ�
	void SetCollisionSphere(const std::shared_ptr<std::vector<std::vector<MSCollisionSphere>>>&pCollisions);
	//�w�肵�����\�[�X�Ƃ̏Փ˔���
	bool CollisionSphere( std::shared_ptr<DX11RenderResource>&pResource);
	void GetCollisionSphere(std::weak_ptr<std::vector<std::vector<MSCollisionSphere>>>&pOutCollisions);

	//���b�V�������擾
	int GetMeshCount();
	//���b�V���ɑ΂���T�u���b�V�������擾
	int GetSubMeshCount(int aMeshIndex);
	//���b�V�����擾
	MSFbxObject* GetMesh();
	//���ߗ��z����擾
	std::vector<float>*GetTransVector();
private:
	//���b�V���f�[�^
	std::shared_ptr<MSFbxObject> mMesh;
	//�����x���(Mesh<SubMesh>)
	std::vector<float>mTransparents;


	//���[���h�s��
	DXWorld mWorld;
	//�r���[�s��
	DXCamera mView;
	//�ˉe�s��
	DXProjection mProj;
	//���E���R���W�����Q��
	std::weak_ptr<std::vector<std::vector<MSCollisionSphere>>>mCollisions;



};