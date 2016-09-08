#pragma once
#include<D3DX9.h>
#include"DX11BaseShader.h"

struct FBXMesh;
struct FBXModelData;
class DX11RenderResource;
class DXDisplay;

class MyDX11Shader :public DX11BaseShader {
public:
	MyDX11Shader();

	struct MyFBXCONSTANTBUFFER1
	{
		D3DXMATRIX mW;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
		D3DXMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
		D3DXVECTOR4 LightDir{ 1,0,-1,0 };
	};

	//���b�V������擾����
	struct MyFBXCONSTANTBUFFER2
	{
		D3DXVECTOR4 Diffuse;
	};



	//���b�V���P�ʂɐݒ�p
	void SetConstantBuffer1(FBXMesh * fbxMesh, DX11RenderResource * resource, DXDisplay*pDisplay);
	void SetConstantBuffer2(FBXModelData*modelData);

private:
	void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout);
};
