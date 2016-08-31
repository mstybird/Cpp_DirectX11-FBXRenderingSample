#pragma once

#include"DX11FbxLoader.h"
#include<D3D11.h>
class DX11FbxManager {
public:
	void Initialize(
		ID3D11Device*pDevice, 
		ID3D11DeviceContext*pDeviceContext,
		ID3D11VertexShader*pVertexShader,
		ID3D11PixelShader*pPixelShader,
		ID3D11InputLayout*pVertexLayout
		);
	//�t�@�C���ǂݍ���
	void LoadFile(std::string pFileName);
	//���b�V���̍X�V(���t���[���K�v)
	void Update();
	//�S�Ẵ��b�V���ɓK�p����f�[�^��o�^
	void SetMatrix(
		D3DXMATRIX matrixW,
		D3DXMATRIX matrixV, 
		D3DXMATRIX matrixP);
	//�����_�����O
	void Render();
	//�������
	void Release();
private:
	struct FBXCONSTANTBUFFER1
	{
		D3DXMATRIX mW;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
		D3DXMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
		D3DXVECTOR4 LightDir{ 1,0,-1,0 };
	};

	struct FBXCONSTANTBUFFER2
	{
		D3DXVECTOR4 Diffuse;
	};

	//�A�v���Ɉ��
	ID3D11Device*mDevice;				//DirectX11�f�o�C�X
	ID3D11DeviceContext*mDeviceContext;	//DirectX11�f�o�C�X�R���e�L�X�g
	//�V�F�[�_���ƂɋL��
	ID3D11VertexShader*mVertexShader;	//���_�V�F�[�_�[
	ID3D11PixelShader*mPixelShader;		//�s�N�Z���V�F�[�_�[
	ID3D11InputLayout*mVertexLayout;	//���_�C���v�b�g���C�A�E�g
	ID3D11Buffer*mConstantBuffer1;		//��x�̂ݓK�p����o�b�t�@
	ID3D11Buffer*mConstantBuffer2;		//���b�V�����ɓK�p����o�b�t�@
	//���f�����ƂɋL��
	DX11FbxLoader*mLoader;				//FBX�ǂݍ��ݗp
	ID3D11Buffer***mVertexBuffer;		//���_�o�b�t�@
	ID3D11Buffer***mIndexBuffer;		//�C���f�b�N�X�o�b�t�@
	std::vector<std::vector<FBXModelData*>>*mMeshData;//���b�V���f�[�^

};

class DX11FbxRender {

};
