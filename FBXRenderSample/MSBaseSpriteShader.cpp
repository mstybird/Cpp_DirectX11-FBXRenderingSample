#include"MSBaseSpriteShader.h"
#include"DX11Resrouce.h"
#include<cassert>
MSBase2DSpriteShader::MSBase2DSpriteShader(const int pConstBufferSize):
	CONSTANTBUFFERSIZE1{pConstBufferSize},
	VERTEXSIZE{sizeof(SpriteVertex)},
	mConstantBuffer{nullptr}
{
}

MSBase2DSpriteShader::~MSBase2DSpriteShader()
{
	SAFE_RELEASE(mConstantBuffer);
}


void MSBase2DSpriteShader::Init()
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

