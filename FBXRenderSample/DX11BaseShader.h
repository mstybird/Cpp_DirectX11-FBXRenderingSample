#pragma once
//�x����\��
#include<string>
#include<D3DX11.h>
#include"DX11FbxManager.h"
#include"DX11RenderResource.h"
#include"DX11FbxResource.h"
#include"DX11Render.h"
#include"DXDisplay.h"
class DX11BaseShader {
protected:
	DX11BaseShader(int constBufferSize1, int constBufferSize2);
public:

	static void Init(ID3D11Device*pDevice, ID3D11DeviceContext*pDeviceContext);
	void Init();
	void InitVertex(
		const std::string&fileName,
		const std::string&EntryPoint = "VS",
		const std::string&Version = "vs_5_0"
	);
	void InitPixel(
		const std::string&fileName,
		const std::string&EntryPointp = "PS",
		const std::string&Version = "ps_5_0"
	);

	virtual void SetConstantBuffer1(DX11RenderResource * resource,DXDisplay*pDisplay) = 0;
	virtual void SetConstantBuffer2(FBXModelData*modelData) = 0;

	friend void DX11Render::Render(DX11FbxManager*fbxManager, DX11RenderResource*resource);
protected:
	//���C�A�E�g�쐬
	virtual void InitLayout();
	ID3D11VertexShader*mVertexShader;	//���_�V�F�[�_�[
	ID3D11PixelShader*mPixelShader;		//�s�N�Z���V�F�[�_�[
	ID3D11InputLayout*mVertexLayout;	//���_�C���v�b�g���C�A�E�g
	//�ꎞ�o�b�t�@
	ID3DBlob*mCompiledShader;
	ID3DBlob*mErrors;
	ID3D11Buffer*mConstantBuffer1;		//��x�̂ݓK�p����o�b�t�@
	ID3D11Buffer*mConstantBuffer2;		//���b�V�����ɓK�p����o�b�t�@
protected:
	const int CONSTANTBUFFERSIZE1;
	const int CONSTANTBUFFERSIZE2;
protected:
	static ID3D11Device*sDevice;				//DirectX11�f�o�C�X
	static ID3D11DeviceContext*sDeviceContext;	//DirectX11�f�o�C�X�R���e�L�X�g
};
