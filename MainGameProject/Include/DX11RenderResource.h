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
	std::unique_ptr<D3DXMATRIX> GetMatrixWVP(const DXDisplay& pDisplay);
	std::unique_ptr<D3DXMATRIX> GetMatrixWVP(const D3DXMATRIX& pAddWorldMatrix, const DXDisplay& pDisplay);


	void SetWorld(const DXWorld&pWorld);
	void SetCamera(const DXCamera&pCamera);
	void SetProjection(const DXProjection&pProjection);
	std::weak_ptr<DXWorld>GetWorld();
	std::weak_ptr<DXCamera>GetCamera();
	std::weak_ptr<DXProjection>GetProjection();

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
	std::shared_ptr<DXWorld>mWorld;
	//�r���[�s��
	std::shared_ptr<DXCamera>mView;
	//�ˉe�s��
	std::shared_ptr<DXProjection>mProj;
	//���E���R���W�����Q��
	std::weak_ptr<std::vector<std::vector<MSCollisionSphere>>>mCollisions;



};