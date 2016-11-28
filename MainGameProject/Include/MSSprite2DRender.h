#pragma once
//2D�X�v���C�g
#include<d3dx11.h>
#include<memory>
class MSBaseSpriteShader;
class MSSpriteBaseResource;
class DX11RenderResource;
class DXDisplay;


class MSSpriteBaseRender abstract{
public:
	MSSpriteBaseRender() :mPrimitiveTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP } {

	}
	//DirectX11�f�o�C�X�̓o�^
	static void Initialize(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext,
		ID3D11RenderTargetView*pRenderTargetView,
		ID3D11DepthStencilView*pDepthStencilView
	);

	virtual void Render(MSSpriteBaseResource&pSprite) = 0;
	//�V�F�[�_�[�̓o�^
	void SetShader(MSBaseSpriteShader&pShader);
protected:

	MSBaseSpriteShader* shader;
	//	DX11BaseSprite*shader;
	const D3D_PRIMITIVE_TOPOLOGY mPrimitiveTopology;
	static ID3D11Device*sDevice;
	static ID3D11DeviceContext*sDeviceContext;
	static ID3D11RenderTargetView*sRenderTargetView;
	static ID3D11DepthStencilView*sDepthStencilView;
};

//2D�X�v���C�g�����_���[
class MSSprite2DRender:public MSSpriteBaseRender {
public:
	void Render(MSSpriteBaseResource &pSprite)override;
	//�r���[�|�[�g�̐ݒ�
	void SetViewPort(D3D11_VIEWPORT*pViewPort);
	//�`���T�C�Y
	D3D11_VIEWPORT*mViewPort;

};

//3D�X�v���C�g�����_���[
class MSSprite3DRender:public MSSpriteBaseRender {
public:
	MSSprite3DRender();
	void SetRenderTarget(const std::weak_ptr<DX11RenderResource>resource);
	void Render(MSSpriteBaseResource&pSprite)override;
private:
	bool mBillBoardFlag;
	std::shared_ptr<DXDisplay>display;

};