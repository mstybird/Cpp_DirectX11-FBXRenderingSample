#include"MySprite2DShader.h"
#include"DX11RenderResource.h"
#include"MSSprite2DResource.h"
#include"DXMatrix.h"
#include<d3dx9.h>
My2DSpriteShader::My2DSpriteShader() :
	MSBase2DSpriteShader(
		sizeof(CONSTANTDATA)
	)
{
}

void My2DSpriteShader::SetConstantBuffer(const std::weak_ptr<MSSprite2DResource>&pResource, D3D11_VIEWPORT & pViewPort)
{
	CONSTANTDATA cb;
	D3D11_MAPPED_SUBRESOURCE pData;

	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

		cb.mW = *pResource.lock()->GetMatrix().lock();
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		cb.ViewPortWidth = pViewPort.Width;
		cb.ViewPortHeight = pViewPort.Height;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer, 0);
	}
}

void My2DSpriteShader::SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>& pLayout)
{
	pLayout.push_back(INPUTLAYOUT_POSITION3D(0));
	pLayout.push_back(INPUTLAYOUT_TEXCOORD2D(12));
}

