#pragma once
#include"MSBaseSpriteShader.h"
#include"DX11Resrouce.h"
//Œ³Sprite2DShader
class My2DSpriteShader :public MSBase2DSpriteShader {
public:
	My2DSpriteShader();

	struct CONSTANTDATA {
		ALIGN16 D3DXMATRIX mW;
		ALIGN16 float ViewPortWidth;
		ALIGN16 float ViewPortHeight;
	};

	void SetConstantBuffer(const std::weak_ptr<MSSprite2DResource>&pResource, D3D11_VIEWPORT&pViewPort)override;


	void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout)override;


};