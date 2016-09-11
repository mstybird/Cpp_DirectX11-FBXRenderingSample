#pragma once
//class DX11SpriteResource;
//とりあえず2Dスプライト
#include<d3dx9.h>
#include"MSBaseShader.h"
#include<memory>
#include<cassert>
class MSSpriteBaseResource;
class DXDisplay;
struct SpriteVertex
{
	D3DXVECTOR2 Pos; //位置
	D3DXVECTOR2 UV; //テクスチャー座標
};

class MSBaseSpriteShader abstract :public MSBaseShader {
public:




	MSBaseSpriteShader(const int pConstBufferSize);
	~MSBaseSpriteShader();

	void Init()override;


	virtual void SetConstantBuffer(const std::weak_ptr<MSSpriteBaseResource>&pResource, std::weak_ptr<DXDisplay>pDisplay,bool pBillBoardFlag) { assert(0); };
	virtual void SetConstantBuffer(const std::weak_ptr<MSSpriteBaseResource>&pResource, D3D11_VIEWPORT&pViewPort) { assert(0); };


	virtual void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout) final;



	ID3D11Buffer*mConstantBuffer;

	//コンスタントバッファの要素一つのサイズ
	const int CONSTANTBUFFERSIZE1;
	//頂点要素一つのサイズ
	const int VERTEXSIZE;
protected:

};

