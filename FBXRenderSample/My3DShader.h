#pragma once
#include<D3DX9.h>

#include"MSBase3DShader.h"
#include"DX11Resrouce.h"
#include<memory>



struct FBXMesh;
struct FBXModelData;
class DX11RenderResource;
class DXDisplay;
class My3DShader :public MSBase3DShader {
public:
	//バッファサイズ、頂点要素サイズを渡す
	My3DShader();

	struct MyFBXCONSTANTBUFFER1
	{
		D3DXMATRIX mW;//ワールド、ビュー、射影の合成変換行列
		D3DXMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
		D3DXVECTOR4 LightDir{ 1,0,-1,0 };
	};

	//メッシュから取得する
	struct MyFBXCONSTANTBUFFER2
	{

		D3DXVECTOR4 Diffuse;
		ALIGN16 float ColorPer;
	};



	//メッシュ単位に設定用
	void SetConstantBuffer1(
		std::weak_ptr<FBXMesh> fbxMesh,
		std::weak_ptr<DX11RenderResource> resource, 
		std::weak_ptr<DXDisplay>pDisplay)override;
	void SetConstantBuffer2(std::weak_ptr<FBXModelData>modelData)override;

private:
	void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout)override;
};
