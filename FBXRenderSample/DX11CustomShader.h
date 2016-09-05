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
		D3DXMATRIX mW;//ワールド、ビュー、射影の合成変換行列
		D3DXMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
		D3DXVECTOR4 LightDir{ 1,0,-1,0 };
	};

	struct MyFBXCONSTANTBUFFER2
	{
		D3DXVECTOR4 Diffuse;
	};



	void SetConstantBuffer1(DX11RenderResource * resource, DXDisplay*pDisplay);
	//行列設定用
	virtual void SetConstantBuffer1_1(FBXMesh * fbxMesh, DX11RenderResource * resource, DXDisplay*pDisplay);
	void SetConstantBuffer2(FBXModelData*modelData);

private:
	void InitLayout();
};
