#pragma once
//class DX11SpriteResource;
//�Ƃ肠����2D�X�v���C�g
#include<d3dx9.h>
#include"MSBaseShader.h"
#include<memory>
#include<cassert>
class MSSpriteBaseResource;
class DXDisplay;
struct SpriteVertex
{
	D3DXVECTOR3 Pos; //�ʒu
	D3DXVECTOR2 UV; //�e�N�X�`���[���W
};

class MSBaseSpriteShader abstract :public MSBaseShader {
public:




	MSBaseSpriteShader(const int pConstBufferSize);
	~MSBaseSpriteShader();

	void Init()override;


	virtual void SetConstantBuffer(MSSpriteBaseResource &pResource, DXDisplay& pDisplay,bool pBillBoardFlag) { assert(0); };
	virtual void SetConstantBuffer(MSSpriteBaseResource &pResource, D3D11_VIEWPORT&pViewPort) { assert(0); };


	virtual void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout) final;



	ID3D11Buffer*mConstantBuffer;

	//�R���X�^���g�o�b�t�@�̗v�f��̃T�C�Y
	const int CONSTANTBUFFERSIZE1;
	//���_�v�f��̃T�C�Y
	const int VERTEXSIZE;
protected:

};

