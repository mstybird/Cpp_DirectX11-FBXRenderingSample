#pragma once
#include"MSBase2DSpriteShader.h"
#include"DX11Resrouce.h"
//��Sprite2DShader
class MySprite2DShader :public MSBaseSpriteShader {
public:
	MySprite2DShader();

	struct CONSTANTDATA {
		ALIGN16 D3DXMATRIX mW;
		ALIGN16 float ViewPortWidth;
		ALIGN16 float ViewPortHeight;
	};

	void SetConstantBuffer(MSSpriteBaseResource&pResource, D3D11_VIEWPORT&pViewPort)override;



};