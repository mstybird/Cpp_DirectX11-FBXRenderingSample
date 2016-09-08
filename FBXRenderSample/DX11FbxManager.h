#pragma once

#include<D3DX11.h>
#include<string>
#include<vector>
/*
	���f���f�[�^�Ƃ��Ă̋@�\�̂�
	�`��ɕK�v�ȍ��W�Ȃǂ̍s��͈�؎c���Ȃ�
*/
struct FBXMesh;
class DX11FbxLoader;
class DX11FbxManager {
public:
	DX11FbxManager();
	~DX11FbxManager();

	//�f�o�C�X�͓o�^������̂���ɓ����Ȃ̂ł��ׂẴN���X�ŋ��ʂɎg����悤�ɂ��Ă���
	static void InitDevice(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext
	);

	void Initialize();
	//�t�@�C���ǂݍ���
	void LoadFile(std::string pFileName, bool animationLoad = true);
	void LoadAnimationFromFile(std::string vfileName);

	//���b�V���̍X�V(���t���[���K�v)
	void Update();

	//�������
	void Release();
	std::vector<FBXMesh*>* GetMeshData();
	ID3D11Buffer*GetVertexBuffer(int i, int j);
	ID3D11Buffer*GetIndexBuffer(int i, int j);
	unsigned int*GetIndexBufferCount(int i,int j);

private:
	//���f�����ƂɋL��
	DX11FbxLoader*mLoader;				//FBX�ǂݍ��ݗp
	ID3D11Buffer***mVertexBuffer;		//���_�o�b�t�@
	ID3D11Buffer***mIndexBuffer;		//�C���f�b�N�X�o�b�t�@
	std::vector<FBXMesh*>*mMeshData;//���b�V���f�[�^
private:
	//static�ϐ�
	//�A�v���Ɉ��
	static ID3D11Device*sDevice;				//DirectX11�f�o�C�X
	static ID3D11DeviceContext*sDeviceContext;	//DirectX11�f�o�C�X�R���e�L�X�g
};

