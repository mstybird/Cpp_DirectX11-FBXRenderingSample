#include"MySprite2DShader.h"
#include"DX11RenderResource.h"
#include"MSSprite2DResource.h"
#include"DXMatrix.h"
#include<d3dx9.h>
MySprite2DShader::MySprite2DShader() :
	MSBaseSpriteShader(
		sizeof(CONSTANTDATA)
	)
{
}

void MySprite2DShader::SetConstantBuffer(MSSpriteBaseResource&pResource, D3D11_VIEWPORT & pViewPort)
{
	CONSTANTDATA cb;
	D3D11_MAPPED_SUBRESOURCE pData;

	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

		cb.mW = *pResource.GetMatrix();
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		cb.ViewPortWidth = pViewPort.Width;
		cb.ViewPortHeight = pViewPort.Height;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer, 0);
	}
}
