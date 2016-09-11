#pragma once
#include"MSBase2DSpriteShader.h"
#include"DX11Resrouce.h"
#include"DXMatrix.h"
class DX11RenderResource;
//3D�X�v���C�g

//�J�X�^��3D�X�v���C�g

class My3DSpriteShader :public MSBaseSpriteShader {
public:
	My3DSpriteShader();

	struct CONSTANTDATA {
		ALIGN16 DXMatrix mWVP;
	};

	void SetConstantBuffer(const std::weak_ptr<MSSpriteBaseResource>&pResource, std::weak_ptr<DXDisplay>pDisplay,bool pBillBoardFlag)override;
};

