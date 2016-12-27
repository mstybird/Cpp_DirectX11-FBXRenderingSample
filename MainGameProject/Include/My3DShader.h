#pragma once

/*
	レイが当たったメッシュのみ透過させるようにする

*/


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

	struct MyFBXCONSTANTBUFFER0//フレーム単位で渡す情報 ライト情報はここに入れる
	{
		D3DXVECTOR4 vDirectLightDir;//ディレクショナルライト
		D3DXVECTOR4 vEye;//カメラ位置
	};

	struct MyFBXCONSTANTBUFFER1
	{
		D3DXMATRIX mW;//ワールド、ビュー、射影の合成変換行列
		D3DXMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
		//D3DXVECTOR4 LightDir{ 1,0,-1,0 };
		ALIGN16 float mAlpha{ 1.0f };
	};

	//メッシュから取得する
	struct MyFBXCONSTANTBUFFER2
	{

		D3DXVECTOR4 Diffuse;
		ALIGN16 float ColorPer;
	};

	virtual void SetConstantBuffer0(CBResource0&aResource)override;

	//メッシュ単位に設定用
	void SetConstantBuffer1(
		FBXMesh& fbxMesh,
		DX11RenderResource* resource, 
		DXDisplay* pDisplay)override;
	void SetConstantBuffer2(std::weak_ptr<FBXModelData>modelData)override;

private:
	void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout)override;
};
