#pragma once
#include"MSBase2DSpriteShader.h"
#include"DX11Resrouce.h"
#include"DXMath.hpp"
class DX11RenderResource;
//3Dスプライト

//カスタム3Dスプライト

class My3DSpriteShader :public MSBaseSpriteShader {
public:
	My3DSpriteShader();

	struct CONSTANTDATA {
		ALIGN16 DXMatrix mWVP;
	};

	void SetConstantBuffer(MSSpriteBaseResource&pResource,DXDisplay& pDisplay,bool pBillBoardFlag)override;
};

