#include"My3DShader.h"
#include"DX11Resrouce.h"
#include"DX11RenderResource.h"
#include"DX11FbxResource.h"
#include"DX11Texture.h"
#include<vector>
#include<iostream>
My3DShader::My3DShader() :
	MSBase3DShader(
		sizeof(MyFBXCONSTANTBUFFER1),
		sizeof(MyFBXCONSTANTBUFFER2),
		sizeof(FbxVertex))
{
}


void My3DShader::SetConstantBuffer1(
	std::weak_ptr<FBXMesh> fbxMesh,
	std::weak_ptr<DX11RenderResource> resource,
	std::weak_ptr<DXDisplay>pDisplay
)
{
	MyFBXCONSTANTBUFFER1 cb;
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

		cb.mW = *fbxMesh.lock()->mWorld * *resource.lock()->GetMatrixWorld().lock();
		
		cb.mWVP = *resource.lock()->GetMatrixWVP(fbxMesh.lock()->mWorld,pDisplay);
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		cb.LightDir = D3DXVECTOR4(1, 0, -1, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer1, 0);
	}
}


void My3DShader::SetConstantBuffer2(std::weak_ptr<FBXModelData> modelData)
{
	D3D11_MAPPED_SUBRESOURCE pData;
	MyFBXCONSTANTBUFFER2 cb;
	auto lSampler = modelData.lock()->Diffuse.lock()->mTexture.lock()->GetSampler();
	auto lTexture = modelData.lock()->Diffuse.lock()->mTexture.lock()->GetTexture();
	sDeviceContext->PSSetSamplers(0, 1, &lSampler);
	sDeviceContext->PSSetShaderResources(0, 1, &lTexture);
	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		cb.Diffuse = modelData.lock()->Diffuse.lock()->Color;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer2, 0);

	}
}

void My3DShader::SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout)
{
	pLayout.push_back(INPUTLAYOUT_POSITION4D(0));
	pLayout.push_back(INPUTLAYOUT_NORMAL(12));
	pLayout.push_back(INPUTLAYOUT_TEXCOORD2D(28));
}
