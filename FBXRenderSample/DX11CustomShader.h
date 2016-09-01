#pragma once
#include<D3DX9.h>
#include"DX11FbxResource.h"
#include"DX11BaseShader.h"
#include"DX11RenderResource.h"
class MyDX11Shader :public DX11BaseShader {
public:
	MyDX11Shader();

	struct MyFBXCONSTANTBUFFER1
	{
		D3DXMATRIX mW;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
		D3DXMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
		D3DXVECTOR4 LightDir{ 1,0,-1,0 };
	};

	struct MyFBXCONSTANTBUFFER2
	{
		D3DXVECTOR4 Diffuse;
	};



	void SetConstantBuffer1(DX11RenderResource * resource, DXDisplay*pDisplay);
	void SetConstantBuffer2(FBXModelData*modelData);

private:
	void InitLayout();
};
