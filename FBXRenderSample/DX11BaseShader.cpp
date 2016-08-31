#include"DX11BaseShader.h"
#include<vector>
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

ID3D11Device*DX11BaseShader::sDevice;				//DirectX11デバイス
ID3D11DeviceContext*DX11BaseShader::sDeviceContext;	//DirectX11デバイスコンテキスト

DX11BaseShader::DX11BaseShader(int constBufferSize1, int constBufferSize2):
	CONSTANTBUFFERSIZE1{ constBufferSize1 },
	CONSTANTBUFFERSIZE2{constBufferSize2}
{
}

void DX11BaseShader::Init(ID3D11Device * pDevice, ID3D11DeviceContext*pDeviceContext)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
	


}

void DX11BaseShader::Init()
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
		return;
	}

	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = CONSTANTBUFFERSIZE2;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(sDevice->CreateBuffer(&cb, NULL, &mConstantBuffer2)))
	{
		//		return E_FAIL;
	}
}

void DX11BaseShader::InitLayout()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC>layout;
	layout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	layout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	layout.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	//インプットレイアウトの作成
	if (FAILED(sDevice->CreateInputLayout(layout.data(), layout.size(), mCompiledShader->GetBufferPointer(), mCompiledShader->GetBufferSize(), &mVertexLayout)))
	{
		return;
	}
}

void DX11BaseShader::InitVertex(const std::string & fileName, const std::string & EntryPoint, const std::string & Version)
{
	if (FAILED(D3DX11CompileFromFile(fileName.c_str(), NULL, NULL, EntryPoint.c_str(), Version.c_str(), 0, 0, NULL, &mCompiledShader, &mErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		return;
	}

	if (FAILED(sDevice->CreateVertexShader(mCompiledShader->GetBufferPointer(), mCompiledShader->GetBufferSize(), NULL, &mVertexShader)))
	{
		SAFE_RELEASE(mCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		return;
	}
	InitLayout();
	SAFE_RELEASE(mErrors);
	SAFE_RELEASE(mCompiledShader);

}

void DX11BaseShader::InitPixel(const std::string & fileName, const std::string & EntryPointp, const std::string & Version)
{
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(fileName.c_str(), NULL, NULL, EntryPointp.c_str(), Version.c_str(), 0, 0, NULL, &mCompiledShader, &mErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		return;
	}
	SAFE_RELEASE(mErrors);
	if (FAILED(sDevice->CreatePixelShader(mCompiledShader->GetBufferPointer(), mCompiledShader->GetBufferSize(), NULL, &mPixelShader)))
	{
		SAFE_RELEASE(mCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		return;
	}
	SAFE_RELEASE(mCompiledShader);
}
