#pragma once
#include"MSBase3DShader.h"
#include<d3dx9.h>
class Collision3DShader :public MSBase3DShader {
public:
	Collision3DShader();

	struct MyFBXCONSTANTBUFFER0//フレーム単位で渡す情報 ライト情報はここに入れる
	{

	};

	struct MyFBXCONSTANTBUFFER1
	{
		D3DXMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
	};

	//メッシュから取得する
	struct MyFBXCONSTANTBUFFER2
	{

	};

	virtual void SetConstantBuffer0(CBResource0&aResource)override;

	//メッシュ単位に設定用
	virtual void SetConstantBuffer1(
		FBXMesh& fbxMesh,
		DX11RenderResource* resource,
		DXDisplay* pDisplay)override;
	virtual void SetConstantBuffer2(std::weak_ptr<FBXModelData>modelData)override;
	void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout)override;

};