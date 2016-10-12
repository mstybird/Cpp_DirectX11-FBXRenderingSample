#pragma once

#include<fbxsdk.h>
#include<D3DX11.h>
#include<string>
#include<vector>
#include<memory>
/*
	���f���f�[�^�Ƃ��Ă̋@�\�̂�
	�`��ɕK�v�ȍ��W�Ȃǂ̍s��͈�؎c���Ȃ�
*/
struct FBXMesh;
class DX11FbxLoader;
class DXVector3;
class MSCollisionSphere;
class DX11RenderResource;
class MSFbxObject;
class MSFbxManager {
public:
	friend class MSFbxObject;
	MSFbxManager();
	~MSFbxManager();

	//�f�o�C�X�͓o�^������̂���ɓ����Ȃ̂ł��ׂẴN���X�ŋ��ʂɎg����悤�ɂ��Ă���
	static void InitDevice(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext
	);

	void Initialize();
	//�t�@�C���ǂݍ���
	void LoadFile(std::string pFileName, bool animationLoad = false);
	void LoadAnimationFromFile(std::string vfileName);
	std::string GetFileName();
	//���b�V���̍X�V(���t���[���K�v)
	bool Update(FbxTime& mCurrentFrame);

	//�������
	void Release();
	std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>> &GetMeshData();


	//���݂̃��b�V�����g���ăR���W�����쐬
	void CreateCollisionSphere();
	//�쐬�ς݂̃R���W���������\�[�X�ɓo�^����
	void RegisterCollision(const std::shared_ptr<DX11RenderResource>&pResource);

	ID3D11Buffer*GetVertexBuffer(int i, int j);
	ID3D11Buffer*GetIndexBuffer(int i, int j);
	unsigned int*GetIndexBufferCount(int i,int j);

	//�����̃��\�b�h�̓��[�u����̂Ŏg�p��A���̃N���X�̊e�C���X�^���X�͋�ɂȂ�
	std::vector<std::shared_ptr<FBXMesh>> MoveMeshData();
	std::vector<std::vector<ID3D11Buffer*>>MoveVertexBuffer();
	std::vector<std::vector<ID3D11Buffer*>>MoveIndexBuffer();

	//�����ς݂̃��b�V�������邩�ǂ���
	bool IsCreatedMeshData();
private:
	//�o�b�t�@���\�[�X���m�ۂ���
	void CreateBuffer();

	std::string mFileName;
	//���f�����ƂɋL��
	//FBX�ǂݍ��ݗp
	DX11FbxLoader*mLoader;				
	
	//(�T�u�܂�)���b�V���̌�
	//�x�N�^�v�f��	=	���b�V����
	//�v�f�̒l		=	�T�u���b�V����)
	std::vector<int>mMeshCount;
	std::vector<std::vector<int>>mMeshVertexCount;
	std::vector<std::vector<int>>mMeshIndexCount;
	std::vector<std::vector<D3D11_BUFFER_DESC>>mMeshVBDesc;
	std::vector<std::vector<D3D11_BUFFER_DESC>>mMeshIBDesc;
	std::vector<std::vector<ID3D11Buffer*>>mVertexBuffer;
	std::vector<std::vector<ID3D11Buffer*>>mIndexBuffer;
	D3D11_BUFFER_DESC mVBDesc;
	D3D11_BUFFER_DESC mIBDesc;

	//�R���W�����f�[�^
	std::shared_ptr<std::vector<std::vector<MSCollisionSphere>>>mCollisions;
	

	std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>>mMeshData;//���b�V���f�[�^
	bool mAnimationFlag;	//�A�j���[�V��������ꍇ��true

private:
	//static�ϐ�
	//�A�v���Ɉ��
	static ID3D11Device*sDevice;				//DirectX11�f�o�C�X
	static ID3D11DeviceContext*sDeviceContext;	//DirectX11�f�o�C�X�R���e�L�X�g
};

