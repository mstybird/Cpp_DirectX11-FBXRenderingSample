#include"MSSprite2DRender.h"

#include"MSBaseSpriteShader.h"
#include"MSSprite2DResource.h"
#include"DX11Texture.h"

ID3D11Device*DX11Sprite2DRender::sDevice;
ID3D11DeviceContext*DX11Sprite2DRender::sDeviceContext;
ID3D11RenderTargetView*DX11Sprite2DRender::sRenderTargetView;
ID3D11DepthStencilView*DX11Sprite2DRender::sDepthStencilView;

void DX11Sprite2DRender::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, ID3D11RenderTargetView * pRenderTargetView, ID3D11DepthStencilView * pDepthStencilView)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
	sRenderTargetView = pRenderTargetView;
	sDepthStencilView = pDepthStencilView;
}

void DX11Sprite2DRender::Render(const std::weak_ptr<MSSprite2DResource>&pSprite)
{

	shader.lock()->mVertexShader.SetShader();
	shader.lock()->mPixelShader.SetShader();
	//頂点インプットレイアウトをセット
	//sDeviceContext->IASetInputLayout(m_pVertexLayout);
	sDeviceContext->IASetPrimitiveTopology(mPrimitiveTopology);
	shader.lock()->SetConstantBuffer(pSprite, *mViewPort);
	sDeviceContext->VSSetConstantBuffers(0, 1, &shader.lock()->mConstantBuffer);
	sDeviceContext->PSSetConstantBuffers(0, 1, &shader.lock()->mConstantBuffer);

	UINT lStride = sizeof(MSBase2DSpriteShader::SpriteVertex);
	UINT lOffset = 0;
	sDeviceContext->IASetVertexBuffers(0, 1, &pSprite.lock()->mVertexBuffer, &lStride, &lOffset);
	;
	sDeviceContext->PSSetSamplers(0, 1, &pSprite.lock()->GetTexture().lock()->mSampleLinear);
	sDeviceContext->PSSetShaderResources(0, 1, &pSprite.lock()->GetTexture().lock()->mTexture);
	sDeviceContext->Draw(4, 0);
}

void DX11Sprite2DRender::SetShader(const std::shared_ptr<MSBase2DSpriteShader>&pShader)
{
	shader = pShader;
}

void DX11Sprite2DRender::SetViewPort(D3D11_VIEWPORT * pViewPort)
{
	mViewPort = pViewPort;
}
