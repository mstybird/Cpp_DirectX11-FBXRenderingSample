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
	cb.ByteWidth = CONSTANTBUFFERSIZE0;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (CONSTANTBUFFERSIZE0 > 0) {
		if (FAILED(sDevice->CreateBuffer(&cb, NULL, &mConstantBuffer0)))
		{
			assert(0);
		}
	}
	else {
		mConstantBuffer0 = nullptr;
	}

	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = CONSTANTBUFFERSIZE1;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (CONSTANTBUFFERSIZE1 > 0) {
		if (FAILED(sDevice->CreateBuffer(&cb, NULL, &mConstantBuffer1)))
		{
			assert(0);
		}
	}
	else {
		mConstantBuffer1 = nullptr;
	}

	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = CONSTANTBUFFERSIZE2;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (CONSTANTBUFFERSIZE2 > 0) {
		if (FAILED(sDevice->CreateBuffer(&cb, NULL, &mConstantBuffer2)))
		{
			assert(0);
		}
	}
	else {
		mConstantBuffer2 = nullptr;
	}
}

void MSBase3DShader::CustomRender(MS3DRender* aRender, GameObjectBase * aObject, const int aIndexX, const int aIndexY)
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
