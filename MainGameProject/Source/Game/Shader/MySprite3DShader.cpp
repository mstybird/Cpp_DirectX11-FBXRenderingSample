#include "MySprite3DShader.h"
#include"MSSprite2DResource.h"
My3DSpriteShader::My3DSpriteShader():
	MSBaseSpriteShader(
		sizeof(CONSTANTDATA)
	) 
{
}

void My3DSpriteShader::SetConstantBuffer(MSSpriteBaseResource&pResource, DXDisplay& pDisplay, bool pBillBoardFlag)
{
	CONSTANTDATA cb;
	D3D11_MAPPED_SUBRESOURCE pData;

	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		pResource.GetMatrixWVP(&cb.mWVP,pDisplay);
		//cb.mW = *pResource.lock()->GetMatrix().lock();
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		//cb.ViewPortWidth = pViewPort.Width;
		//cb.ViewPortHeight = pViewPort.Height;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer, 0);
	}
}
