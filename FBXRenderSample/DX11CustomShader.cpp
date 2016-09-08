#include"DX11CustomShader.h"
#include"DX11Resrouce.h"
#include"DX11RenderResource.h"
#include"DX11FbxResource.h"
#include"DX11Texture.h"
#include<vector>
#include<iostream>
MyDX11Shader::MyDX11Shader() :
	DX11BaseShader(
		sizeof(MyFBXCONSTANTBUFFER1),
		sizeof(MyFBXCONSTANTBUFFER2),
		sizeof(FbxVertex))
{
}


void MyDX11Shader::SetConstantBuffer1(FBXMesh * fbxMesh, DX11RenderResource * resource, DXDisplay * pDisplay)
{
	MyFBXCONSTANTBUFFER1 cb;
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

		cb.mW = *fbxMesh->mWorld * *resource->GetMatrixWorld();
		
		cb.mWVP = resource->GetMatrixWVP(fbxMesh->mWorld.get(),pDisplay);
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		cb.LightDir = D3DXVECTOR4(1, 0, -1, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer1, 0);
	}
}


void MyDX11Shader::SetConstantBuffer2(FBXModelData * modelData)
{
	D3D11_MAPPED_SUBRESOURCE pData;
	MyFBXCONSTANTBUFFER2 cb;
	auto lSampler = modelData->Diffuse.lock()->mTexture.lock()->GetSampler();
	auto lTexture = modelData->Diffuse.lock()->mTexture.lock()->GetTexture();
	sDeviceContext->PSSetSamplers(0, 1, &lSampler);
	sDeviceContext->PSSetShaderResources(0, 1, &lTexture);
	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		cb.Diffuse = modelData->Diffuse.lock()->Color;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer2, 0);

	}
}

void MyDX11Shader::SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout)
{
	pLayout.push_back(INPUTLAYOUT_POSITION4D(0));
	pLayout.push_back(INPUTLAYOUT_NORMAL(12));
	pLayout.push_back(INPUTLAYOUT_TEXCOORD2D(28));
}
