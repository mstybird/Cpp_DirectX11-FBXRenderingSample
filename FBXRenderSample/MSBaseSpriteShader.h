#pragma once
//class DX11SpriteResource;
//とりあえず2Dスプライト
#include<d3dx9.h>
#include"MSBaseShader.h"
#include<memory>
class MSSprite2DResource;
class MSBase2DSpriteShader abstract :public MSBaseShader {
public:

	struct SpriteVertex
	{
		D3DXVECTOR3 Pos; //位置
		D3DXVECTOR2 UV; //テクスチャー座標
	};


	MSBase2DSpriteShader(const int pConstBufferSize);
	~MSBase2DSpriteShader();

	void Init()override;


	virtual void SetConstantBuffer(const std::weak_ptr<MSSprite2DResource>&pResource, D3D11_VIEWPORT&pViewPort) = 0;


	virtual void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout) = 0;



	ID3D11Buffer*mConstantBuffer;

	//コンスタントバッファの要素一つのサイズ
	const int CONSTANTBUFFERSIZE1;
	//頂点要素一つのサイズ
	const int VERTEXSIZE;
protected:

};

