#pragma once

#include<fbxsdk.h>
#include<D3DX11.h>
#include<string>
#include<vector>
#include<memory>
#include<unordered_map>
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

struct MeshBuffer {
	std::vector<std::vector<ID3D11Buffer*>>mVertexBuffer;
	std::vector<std::vector<ID3D11Buffer*>>mVertexCollisionBuffer;
	std::vector<std::vector<ID3D11Buffer*>>mIndexBuffer;
	std::vector<std::vector<uint32_t>>mIndexLength;
	//�R���W�����f�[�^
	std::vector<std::vector<MSCollisionSphere>>mCollisions;
	std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>>mMeshData;//���b�V���f�[�^
	~MeshBuffer();
};

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
	//���b�V�������L���b�V������
	void RegisterMesh(FbxTime& mCurrentFrame, const int pAnimationIndex);

	void LoadAnimationFromFile(std::string vfileName);
	std::string GetFileName();
	//���b�V���̍X�V(���t���[���K�v)
	bool Update(FbxTime& mCurrentFrame, const int pAnimationIndex);

	//�������
	void Release();

	//���݂̃��b�V�����g���ăR���W�����쐬
	void CreateCollisionSphere(FbxTime& mCurrentFrame, const int pAnimationIndex);
	//�쐬�ς݂̃R���W���������\�[�X�ɓo�^����
	//void RegisterCollision(const std::shared_ptr<DX11RenderResource>&pResource);

	std::vector<std::shared_ptr<FBXMesh>>*GetMeshData(int aAnimation, FbxTime aTime);
	std::vector<std::vector<ID3D11Buffer*>>*GetVertexBuffer(int aAnimation, FbxTime aTime);
	std::vector<std::vector<ID3D11Buffer*>>*GetVertexCollisionBuffer(int aAnimation,FbxTime aTime);
	std::vector<std::vector<ID3D11Buffer*>>*GetIndexBuffer(int aAnimation, FbxTime aTime);
	std::vector<std::vector<uint32_t>>*GetIndexBufferCount(int aAnimation, FbxTime aTime);
	std::vector<std::vector<MSCollisionSphere>>*GetCollisionSphere(int aAnimation, FbxTime aTime);

	//�����̃��\�b�h�̓��[�u����̂Ŏg�p��A���̃N���X�̊e�C���X�^���X�͋�ɂȂ�
	//�L���b�V�����邽�ߕs�v�ƂȂ�
	//std::vector<std::shared_ptr<FBXMesh>> MoveMeshData();
	//std::vector<std::vector<ID3D11Buffer*>>MoveVertexBuffer();
	//std::vector<std::vector<ID3D11Buffer*>>MoveIndexBuffer();

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

	//�A�j���[�V�����ƃA�j���[�V�����^�C�����Ƃ̃o�b�t�@
	std::unordered_map<int,std::unordered_map<LONG64, MeshBuffer>> mMeshBuffer;
	std::vector<std::vector<int>>mMeshVertexCount;
	std::vector<std::vector<int>>mMeshIndexCount;
	//std::vector<std::vector<D3D11_BUFFER_DESC>>mMeshVBDesc;
	//std::vector<std::vector<D3D11_BUFFER_DESC>>mMeshIBDesc;
	//D3D11_BUFFER_DESC mVBDesc;
	//D3D11_BUFFER_DESC mIBDesc;

	




	bool mAnimationFlag;	//�A�j���[�V��������ꍇ��true

private:
	//static�ϐ�
	//�A�v���Ɉ��
	static ID3D11Device*sDevice;				//DirectX11�f�o�C�X
	static ID3D11DeviceContext*sDeviceContext;	//DirectX11�f�o�C�X�R���e�L�X�g
};

