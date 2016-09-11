#pragma once
#include"MSBase2DSpriteShader.h"
#include"DX11Resrouce.h"
//Œ³Sprite2DShader
class My2DSpriteShader :public MSBaseSpriteShader {
public:
	My2DSpriteShader();

	struct CONSTANTDATA {
		ALIGN16 D3DXMATRIX mW;
		ALIGN16 float ViewPortWidth;
		ALIGN16 float ViewPortHeight;
	};

	void SetConstantBuffer(const std::weak_ptr<MSSpriteBaseResource>&pResource, D3D11_VIEWPORT&pViewPort)override;



};