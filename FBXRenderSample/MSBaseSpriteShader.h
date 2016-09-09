#pragma once
//class DX11SpriteResource;
//�Ƃ肠����2D�X�v���C�g
#include<d3dx9.h>
#include"MSBaseShader.h"
#include<memory>
class MSSprite2DResource;
class MSBase2DSpriteShader abstract :public MSBaseShader {
public:

	struct SpriteVertex
	{
		D3DXVECTOR3 Pos; //�ʒu
		D3DXVECTOR2 UV; //�e�N�X�`���[���W
	};


	MSBase2DSpriteShader(const int pConstBufferSize);
	~MSBase2DSpriteShader();

	void Init()override;


	virtual void SetConstantBuffer(const std::weak_ptr<MSSprite2DResource>&pResource, D3D11_VIEWPORT&pViewPort) = 0;


	virtual void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout) = 0;



	ID3D11Buffer*mConstantBuffer;

	//�R���X�^���g�o�b�t�@�̗v�f��̃T�C�Y
	const int CONSTANTBUFFERSIZE1;
	//���_�v�f��̃T�C�Y
	const int VERTEXSIZE;
protected:

};

