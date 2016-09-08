#pragma once
#include<d3dx9.h>
#include<D3DX11.h>
class DX11FbxManager;
class DX11RenderResource;
class DX11BaseShader;
class DXDisplay;
//#include"DXDisplay.h"
//DirectX11�ŕ`�悷��N���X
class DX11Render {
public:
	//�g�|���W�[�̏�����
	DX11Render();
	~DX11Render();
	//DirectX11�f�o�C�X�̓o�^
	static void Initialize(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext,
		ID3D11RenderTargetView*sRenderTargetView,
		ID3D11DepthStencilView*sDepthStencilView
		);
	//��ʃN���A
	static void Clear(D3DXVECTOR4 pColor);

	//�����_�����O
	void Render(DX11FbxManager*fbxManager,DX11RenderResource*resource);

	//�`����(�r���[�s��Ǝˉe�s��)��ݒ�
	void SetRenderTarget(DX11RenderResource*resource);
	//�V�F�[�_�[�̓o�^
	void SetShader(DX11BaseShader*pShader);
protected:

	//�����_�����O�Ɏg�p����V�F�[�_�[
	DX11BaseShader*shader;
	DXDisplay*display;
	const D3D_PRIMITIVE_TOPOLOGY mPrimitiveTopology;

	 static ID3D11Device*sDevice;				//DirectX11�f�o�C�X
	 static ID3D11DeviceContext*sDeviceContext;	//DirectX11�f�o�C�X�R���e�L�X�g
	 static ID3D11RenderTargetView*sRenderTargetView;
	 static ID3D11DepthStencilView*sDepthStencilView;

};


