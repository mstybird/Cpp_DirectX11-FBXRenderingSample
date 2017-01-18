#pragma once
#include"MSBase2DSpriteShader.h"
#include"DX11Resrouce.h"
//元Sprite2DShader
class MySprite2DShader :public MSBaseSpriteShader {
public:
	MySprite2DShader();

	struct CONSTANTDATA {
		ALIGN16 D3DXMATRIX mW;			//変換行列
		ALIGN16 float ViewPortWidth;	//ビューポート
		ALIGN16 float ViewPortHeight;	//ビューポート
		ALIGN16 float Transparent;		//透過率
	};

	void SetConstantBuffer(MSSpriteBaseResource&pResource, D3D11_VIEWPORT&pViewPort)override;



};