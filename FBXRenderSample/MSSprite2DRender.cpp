#include"MSSprite2DRender.h"

#include"MSBase2DSpriteShader.h"
#include"MSSprite2DResource.h"
#include"DX11TextureManager.hpp"
#include"DXDisplay.h"
#include"DX11RenderResource.h"
ID3D11Device*MSSpriteBaseRender::sDevice;
ID3D11DeviceContext*MSSpriteBaseRender::sDeviceContext;
ID3D11RenderTargetView*MSSpriteBaseRender::sRenderTargetView;
ID3D11DepthStencilView*MSSpriteBaseRender::sDepthStencilView;

void MSSpriteBaseRender::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, ID3D11RenderTargetView * pRenderTargetView, ID3D11DepthStencilView * pDepthStencilView)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
	sRenderTargetView = pRenderTargetView;
	sDepthStencilView = pDepthStencilView;
}

void MSSprite2DRender::Render(MSSpriteBaseResource&pSprite)
{

	shader->mVertexShader.SetShader();
	shader->mPixelShader.SetShader();
	//頂点インプットレイアウトをセット
	//sDeviceContext->IASetInputLayout(m_pVertexLayout);
	sDeviceContext->IASetPrimitiveTopology(mPrimitiveTopology);
	shader->SetConstantBuffer(pSprite, *mViewPort);
	sDeviceContext->VSSetConstantBuffers(0, 1, &shader->mConstantBuffer);
	sDeviceContext->PSSetConstantBuffers(0, 1, &shader->mConstantBuffer);

	UINT lStride = sizeof(SpriteVertex);
	UINT lOffset = 0;
	sDeviceContext->IASetVertexBuffers(0, 1, &pSprite.mVertexBuffer, &lStride, &lOffset);
	;
	sDeviceContext->PSSetSamplers(0, 1, &pSprite.GetTexture()->mSampleLinear);
	sDeviceContext->PSSetShaderResources(0, 1, &pSprite.GetTexture()->mTexture);
	sDeviceContext->Draw(4, 0);
}

void MSSpriteBaseRender::SetShader(MSBaseSpriteShader&pShader)
{
	shader = &pShader;
}

void MSSprite2DRender::SetViewPort(D3D11_VIEWPORT * pViewPort)
{
	mViewPort = pViewPort;
}

MSSprite3DRender::MSSprite3DRender():
	display{std::make_shared<DXDisplay>()}
{
}

void MSSprite3DRender::SetRenderTarget(const std::weak_ptr<DX11RenderResource> resource)
{
	display->SetRenderTarget(
		resource.lock()->GetCamera(),
		resource.lock()->GetProjection()
	);
}

void MSSprite3DRender::Render(MSSpriteBaseResource& pSprite)
{
	shader->mVertexShader.SetShader();
	shader->mPixelShader.SetShader();
	sDeviceContext->IASetPrimitiveTopology(mPrimitiveTopology);
	shader->SetConstantBuffer(pSprite,*display,mBillBoardFlag);
	sDeviceContext->VSSetConstantBuffers(0, 1, &shader->mConstantBuffer);
	sDeviceContext->PSSetConstantBuffers(0, 1, &shader->mConstantBuffer);
	UINT lStride = sizeof(SpriteVertex);
	UINT lOffset = 0;
	sDeviceContext->IASetVertexBuffers(0, 1, &pSprite.mVertexBuffer, &lStride, &lOffset);
	;
	sDeviceContext->PSSetSamplers(0, 1, &pSprite.GetTexture()->mSampleLinear);
	sDeviceContext->PSSetShaderResources(0, 1, &pSprite.GetTexture()->mTexture);
	sDeviceContext->Draw(4, 0);
}
