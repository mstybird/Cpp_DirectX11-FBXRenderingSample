#pragma once
//DirectX11�ŕ`�悷��N���X
#include<D3DX11.h>
#include"DX11RenderResource.h"
#include"DX11FbxManager.h"
#include"DXDisplay.h"
class DX11BaseShader;
class DX11Render {

public:
	//DirectX11�f�o�C�X�̓o�^
	static void Initialize(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext,
		ID3D11RenderTargetView*sRenderTargetView,
		ID3D11DepthStencilView*sDepthStencilView
		);
	//��ʃN���A
	static void Clear(D3DXVECTOR4 pColor);
	//FBX���f���̃����_�����O
	void Render(DX11FbxManager*fbxManager,DX11RenderResource*resource);
	//�r���[�ˉe�s��̓o�^
	void SetRenderTarget(DX11RenderResource*resource);
	//�V�F�[�_�[�̓o�^
	void SetShader(DX11BaseShader*pShader);
	//�����_�����O�Ɏg�p����V�F�[�_�[
	DX11BaseShader*shader;
	DXDisplay display;
private:
	//�ǂ̃V�F�[�_���g����
private:
	 static ID3D11Device*sDevice;				//DirectX11�f�o�C�X
	 static ID3D11DeviceContext*sDeviceContext;	//DirectX11�f�o�C�X�R���e�L�X�g
	 static ID3D11RenderTargetView*sRenderTargetView;
	 static ID3D11DepthStencilView*sDepthStencilView;

};

#include"DX11BaseShader.h"