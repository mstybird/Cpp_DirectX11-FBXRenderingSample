#include"DX11CustomShader.h"
#include"DX11Resrouce.h"
#include<vector>
#include<iostream>
MyDX11Shader::MyDX11Shader() :
	DX11BaseShader(sizeof(MyFBXCONSTANTBUFFER1), sizeof(MyFBXCONSTANTBUFFER2))
{
}

void MyDX11Shader::SetConstantBuffer1(DX11RenderResource * resource, DXDisplay*pDisplay)
{
	
	D3D11_MAPPED_SUBRESOURCE pData;
	MyFBXCONSTANTBUFFER1 cb;
	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		cb.mW = *resource->GetMatrixWorld();
		cb.mWVP = resource->GetMatrixWVP(pDisplay);
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
	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		cb.Diffuse = modelData->Diffuse->Color;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer2, 0);

	}
}

void MyDX11Shader::InitLayout()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC>layout;

	layout.push_back(INPUTLAYOUT_POSITION(0));
	layout.push_back(INPUTLAYOUT_NORMAL(16));
	layout.push_back(INPUTLAYOUT_TEXCOORD(28));

	//インプットレイアウトの作成
	if (FAILED(sDevice->CreateInputLayout(layout.data(), layout.size(), mCompiledShader->GetBufferPointer(), mCompiledShader->GetBufferSize(), &mVertexLayout)))
	{
		return;
	}
}
