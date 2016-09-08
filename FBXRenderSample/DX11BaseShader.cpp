#include"DX11BaseShader.h"
#include"DX11Resrouce.h"
#include"DX11FbxManager.h"
#include"DX11RenderResource.h"
#include"DX11FbxResource.h"
#include"DX11Render.h"
#include"DXDisplay.h"
#include"DX11Sprite2D.h"
#include<vector>
#include<cassert>
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

ID3D11Device*DX11BaseShader::sDevice;				//DirectX11デバイス
ID3D11DeviceContext*DX11BaseShader::sDeviceContext;	//DirectX11デバイスコンテキスト

DX11BaseShader::DX11BaseShader(int constBufferSize1, int constBufferSize2, unsigned int constVertexSize) :
	CONSTANTBUFFERSIZE1{ constBufferSize1 },
	CONSTANTBUFFERSIZE2{ constBufferSize2 },
	VERTEXSIZE{ constVertexSize }
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


void DX11BaseShader::InitVertex(const std::string & fileName, const std::string & EntryPoint, const std::string & Version)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC>layout;

	SetLayout(layout);

	mVertexShader.Create(
		fileName,
		EntryPoint,
		Version,
		layout
		);

}

void DX11BaseShader::InitPixel(const std::string & fileName, const std::string & EntryPointp, const std::string & Version)
{
	mPixelShader.Create(fileName,EntryPointp,Version);
}

MSVertexShader * DX11BaseShader::GetVS()
{
	return &mVertexShader;
}

MSPixelShader * DX11BaseShader::GetPS()
{
	return &mPixelShader;
}

ID3D11Buffer ** DX11BaseShader::GetCB1()
{
	return &mConstantBuffer1;
}

ID3D11Buffer ** DX11BaseShader::GetCB2()
{
	return &mConstantBuffer2;
}

const unsigned int DX11BaseShader::GetVertexSize()
{
	return VERTEXSIZE;
}

ID3D11Device*MSVertexShader::sDevice;
ID3D11DeviceContext*MSVertexShader::sDeviceContext;
void MSVertexShader::sInitialize(ID3D11Device * pDevice, ID3D11DeviceContext*pDeviceContext)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
}
MSVertexShader::MSVertexShader():
	mVertexShader{nullptr},
	mVertexLayout{nullptr}
{

}

MSVertexShader::~MSVertexShader()
{
	SAFE_RELEASE(mVertexShader);
	SAFE_RELEASE(mVertexLayout);
	SAFE_RELEASE(mCompiledShader);
	SAFE_RELEASE(mErrors);
}

void MSVertexShader::Create(const std::string & fileName, const std::string & EntryPoint, const std::string & Version, vector<D3D11_INPUT_ELEMENT_DESC> &pLayout)
{
	//頂点シェーダ作成
	{
		if (FAILED(D3DX11CompileFromFile(fileName.c_str(), NULL, NULL, EntryPoint.c_str(), Version.c_str(), 0, 0, NULL, &mCompiledShader, &mErrors, NULL)))
		{
			//MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
			assert(0);
			return;
		}
		if (FAILED(sDevice->CreateVertexShader(mCompiledShader->GetBufferPointer(), mCompiledShader->GetBufferSize(), NULL, &mVertexShader)))
		{
			SAFE_RELEASE(mCompiledShader);
			//MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
			assert(0);
			return;
		}

	}

	//頂点インプットレイアウト作成
	{
		if (FAILED(sDevice->CreateInputLayout(pLayout.data(), pLayout.size(), mCompiledShader->GetBufferPointer(), mCompiledShader->GetBufferSize(), &mVertexLayout)))
		{
			assert(0);
		}

	}



	SAFE_RELEASE(mErrors);
	SAFE_RELEASE(mCompiledShader);

}

ID3D11VertexShader* MSVertexShader::GetShader()const
{
	return mVertexShader;
}

ID3D11InputLayout * MSVertexShader::GetLayout() const
{
	return mVertexLayout;
}

void MSVertexShader::SetShader()
{
	sDeviceContext->VSSetShader(mVertexShader,nullptr, 0);
	sDeviceContext->IASetInputLayout(mVertexLayout);
}

ID3D11Device*MSPixelShader::sDevice{ nullptr };
ID3D11DeviceContext*MSPixelShader::sDeviceContext{ nullptr };
void MSPixelShader::sInitialize(ID3D11Device * pDevice, ID3D11DeviceContext*pDeviceContext)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
}

MSPixelShader::MSPixelShader()
{
}

MSPixelShader::~MSPixelShader()
{
	SAFE_RELEASE(mPixelShader);
	SAFE_RELEASE(mCompiledShader);
	SAFE_RELEASE(mErrors);

}

void MSPixelShader::Create(const std::string & pFileName, const std::string & pEntryPoint, const std::string & pVersion)
{
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(pFileName.c_str(), NULL, NULL, pEntryPoint.c_str(), pVersion.c_str(), 0, 0, NULL, &mCompiledShader, &mErrors, NULL)))
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
	//	mPixelShader.reset(lPixelShader);
	SAFE_RELEASE(mCompiledShader);
}

ID3D11PixelShader * MSPixelShader::GetShader() const
{
	return mPixelShader;
}

void MSPixelShader::SetShader()
{
	sDeviceContext->PSSetShader(mPixelShader, nullptr, 0);
}


ID3D11Device*DX11BaseSprite::sDevice{ nullptr };
ID3D11DeviceContext*DX11BaseSprite::sDeviceContext{ nullptr };
DX11BaseSprite::DX11BaseSprite():
	CONSTANTBUFFERSIZE1{sizeof(CONSTANTDATA)}
{
}

DX11BaseSprite::~DX11BaseSprite()
{
	SAFE_RELEASE(mConstantBuffer);
}



void DX11BaseSprite::Init(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
}

void DX11BaseSprite::Init()
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

void DX11BaseSprite::InitVertex(const std::string & fileName, const std::string & EntryPoint, const std::string & Version)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC>layout;

	SetLayout(layout);

	mVertexShader.Create(
		fileName,
		EntryPoint,
		Version,
		layout
		);

}

void DX11BaseSprite::InitPixel(const std::string & fileName, const std::string & EntryPointp, const std::string & Version)
{
	mPixelShader.Create(fileName, EntryPointp, Version);
}

void DX11BaseSprite::SetConstantBuffer(DX11SpriteResource & pResource)
{
	CONSTANTDATA cb;
	D3D11_MAPPED_SUBRESOURCE pData;

	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {


		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer, 0);
	}
}

void DX11BaseSprite::SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>& pLayout)
{
	pLayout.push_back(INPUTLAYOUT_POSITION3D(0));
	pLayout.push_back(INPUTLAYOUT_TEXCOORD2D(12));
}
