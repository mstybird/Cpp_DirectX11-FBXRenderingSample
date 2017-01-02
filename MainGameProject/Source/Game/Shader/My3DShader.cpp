#include"My3DShader.h"
#include"DX11Resrouce.h"
#include"DX11RenderResource.h"
#include<DX11FBXLoader.hpp>
#include"DX11TextureManager.hpp"
#include"CBResource0.h"
#include<vector>
#include<iostream>
My3DShader::My3DShader() :
	MSBase3DShader(
		sizeof(MyFBXCONSTANTBUFFER0),
		sizeof(MyFBXCONSTANTBUFFER1),
		sizeof(MyFBXCONSTANTBUFFER2),
		sizeof(FbxVertex))
{
}


void My3DShader::SetConstantBuffer0(CBResource0 & aResource)
{
	MyFBXCONSTANTBUFFER0 cb;
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

		cb.vDirectLightDir = { 1,1,0,0 };
		cb.vEye = aResource.vEye;
		cb.vEye.w = 0;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer0, 0);
	}
}

void My3DShader::SetConstantBuffer1(
	FBXMesh& fbxMesh,
	DX11RenderResource* resource,
	DXDisplay* pDisplay
)
{
	MyFBXCONSTANTBUFFER1 cb;
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		
		cb.mW = *fbxMesh.mWorld * *resource->GetMatrixWorld();
		resource->GetMatrixWVP(&cb.mWVP,*fbxMesh.mWorld,*pDisplay);
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		//if(resource->GetTransVector()->size()>0){
		//	cb.mAlpha = resource->GetTransVector()->at(0);
		//}
		//else {
		//	cb.mAlpha = 1.0f;
		//}
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer1, 0);
	}
}


void My3DShader::SetConstantBuffer2(std::weak_ptr<FBXModelData> modelData)
{
	D3D11_MAPPED_SUBRESOURCE pData;
	MyFBXCONSTANTBUFFER2 cb;

	ID3D11SamplerState*lSampler{ nullptr };
	ID3D11ShaderResourceView*lTexture{ nullptr };
	//�e�N�X�`�����������ꍇ�̂݃Z�b�g����
	if (modelData.lock()->Diffuse.lock()->mTexture) {
		lSampler = modelData.lock()->Diffuse.lock()->mTexture->GetSampler();
		lTexture = modelData.lock()->Diffuse.lock()->mTexture->GetTexture();
	}
	else {
	}
	//cb.Ambient = modelData.lock()->Ambient.lock()->Color;
	//cb.Diffuse = modelData.lock()->Diffuse.lock()->Color;
	//cb.Specular = modelData.lock()->Specular.lock()->Color;

	cb.Ambient = D3DXVECTOR4( 0.3,0.3,0.3,1.0 );
	cb.Diffuse = D3DXVECTOR4(1,1,1,1);
	cb.Specular = D3DXVECTOR4(0.2,0.2,0.2,1.0);

	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		memcpy(pData.pData,(void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer2, 0);

	}
	sDeviceContext->PSSetSamplers(0, 1, &lSampler);
	sDeviceContext->PSSetShaderResources(0, 1, &lTexture);
}

void My3DShader::SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout)
{
	pLayout.push_back(INPUTLAYOUT_POSITION3D(0));
	pLayout.push_back(INPUTLAYOUT_NORMAL(12));
	pLayout.push_back(INPUTLAYOUT_TEXCOORD2D(24));
}
