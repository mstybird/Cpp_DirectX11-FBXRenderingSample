#pragma once
#include"MSBase2DSpriteShader.h"
#include"DX11Resrouce.h"
#include"DXMath.hpp"
class DX11RenderResource;
//3D�X�v���C�g

//�J�X�^��3D�X�v���C�g

class My3DSpriteShader :public MSBaseSpriteShader {
public:
	My3DSpriteShader();

	struct CONSTANTDATA {
		ALIGN16 DXMatrix mWVP;
	};

	void SetConstantBuffer(MSSpriteBaseResource&pResource,DXDisplay& pDisplay,bool pBillBoardFlag)override;
};

