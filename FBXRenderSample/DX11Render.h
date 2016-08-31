#pragma once
//DirectX11�ŕ`�悷��N���X
#include<D3DX11.h>
#include"DX11RenderResource.h"
#include"DX11FbxManager.h"
class DX11BaseShader;
class DX11Render {

public:
	static void Initialize(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext);
	void Render(DX11FbxManager*fbxManager,DX11RenderResource*resource);
	DX11BaseShader*shader;
private:
	//�ǂ̃V�F�[�_���g����
private:
	 static ID3D11Device*sDevice;				//DirectX11�f�o�C�X
	 static ID3D11DeviceContext*sDeviceContext;	//DirectX11�f�o�C�X�R���e�L�X�g

};

#include"DX11BaseShader.h"