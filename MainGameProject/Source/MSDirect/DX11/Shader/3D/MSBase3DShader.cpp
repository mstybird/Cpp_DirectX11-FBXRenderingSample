#include"MSBase3DShader.h"
#include<cassert>



MSBase3DShader::MSBase3DShader(int constBufferSize0, int constBufferSize1, int constBufferSize2, unsigned int constVertexSize) :
	CONSTANTBUFFERSIZE0{ constBufferSize0 },
	CONSTANTBUFFERSIZE1{ constBufferSize1 },
	CONSTANTBUFFERSIZE2{ constBufferSize2 },
	VERTEXSIZE{ constVertexSize }
{
}



void MSBase3DShader::Init()
{
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = CONSTANTBUFFERSIZE1;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(sDevice->CreateBuffer(&cb, NULL, &mConstantBuffer1)))
	{
		assert(0);
	}

	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = CONSTANTBUFFERSIZE2;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(sDevice->CreateBuffer(&cb, NULL, &mConstantBuffer2)))
	{
		assert(0);
	}
}

void MSBase3DShader::CustomRender(GameObjectBase * aObject)
{
}

MSVertexShader * MSBase3DShader::GetVS()
{
	return &mVertexShader;
}

MSPixelShader * MSBase3DShader::GetPS()
{
	return &mPixelShader;
}

ID3D11Buffer ** MSBase3DShader::GetCB0()
{
	return &mConstantBuffer0;
}

ID3D11Buffer ** MSBase3DShader::GetCB1()
{
	return &mConstantBuffer1;
}

ID3D11Buffer ** MSBase3DShader::GetCB2()
{
	return &mConstantBuffer2;
}

const unsigned int MSBase3DShader::GetVertexSize()
{
	return VERTEXSIZE;
}
