#include "3DCollisionShader.h"
#include<DX11FBXLoader.hpp>
#include"CBResource0.h"
#include"DX11RenderResource.h"
#include"DX11Resrouce.h"
Collision3DShader::Collision3DShader():
	MSBase3DShader(
		0,
		sizeof(MyFBXCONSTANTBUFFER1),
		0,
		sizeof(FbxVertex))
{
}

void Collision3DShader::SetConstantBuffer0(CBResource0 & aResource)
{
	return;
}

void Collision3DShader::SetConstantBuffer1(FBXMesh & fbxMesh, DX11RenderResource * resource, DXDisplay * pDisplay)
{
	MyFBXCONSTANTBUFFER1 cb;
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

		resource->GetMatrixWVP(&cb.mWVP, *fbxMesh.mWorld, *pDisplay);
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer1, 0);
	}
	return;
}

void Collision3DShader::SetConstantBuffer2(std::weak_ptr<FBXModelData> modelData)
{
	return;
}

void Collision3DShader::SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>& pLayout)
{
	pLayout.push_back(INPUTLAYOUT_POSITION3D(0));
}
