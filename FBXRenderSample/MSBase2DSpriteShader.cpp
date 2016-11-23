#include"MSBase2DSpriteShader.h"
#include"DX11Resrouce.h"
#include<cassert>
MSBaseSpriteShader::MSBaseSpriteShader(const int pConstBufferSize):
	CONSTANTBUFFERSIZE1{pConstBufferSize},
	VERTEXSIZE{sizeof(SpriteVertex)},
	mConstantBuffer{nullptr}
{
}

MSBaseSpriteShader::~MSBaseSpriteShader()
{
	SAFE_RELEASE(mConstantBuffer);
}


void MSBaseSpriteShader::Init()
{
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = CONSTANTBUFFERSIZE1;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(sDevice->CreateBuffer(&cb, NULL, &mConstantBuffer)))
	{
		assert(0);
	}
}

void MSBaseSpriteShader::SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>& pLayout)
{
	pLayout.push_back(INPUTLAYOUT_POSITION3D(0));
	pLayout.push_back(INPUTLAYOUT_TEXCOORD2D(12));
}

