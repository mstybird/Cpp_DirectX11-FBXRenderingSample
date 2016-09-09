#pragma once
//2D�X�v���C�g
#include<d3dx11.h>
#include<memory>
class MSBase2DSpriteShader;
class MSSprite2DResource;
class DX11Sprite2DRender {
public:
	DX11Sprite2DRender() :mPrimitiveTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP } {

	}
	//DirectX11�f�o�C�X�̓o�^
	static void Initialize(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext,
		ID3D11RenderTargetView*pRenderTargetView,
		ID3D11DepthStencilView*pDepthStencilView
	);

	void Render(const std::weak_ptr<MSSprite2DResource>&pSprite);
	//�V�F�[�_�[�̓o�^
	void SetShader(const std::shared_ptr<MSBase2DSpriteShader>&pShader);
	//�r���[�|�[�g�̐ݒ�
	void SetViewPort(D3D11_VIEWPORT*pViewPort);
private:
	//�`���T�C�Y
	D3D11_VIEWPORT*mViewPort;

	std::weak_ptr<MSBase2DSpriteShader>shader;
	//	DX11BaseSprite*shader;
	const D3D_PRIMITIVE_TOPOLOGY mPrimitiveTopology;
	static ID3D11Device*sDevice;
	static ID3D11DeviceContext*sDeviceContext;
	static ID3D11RenderTargetView*sRenderTargetView;
	static ID3D11DepthStencilView*sDepthStencilView;
};