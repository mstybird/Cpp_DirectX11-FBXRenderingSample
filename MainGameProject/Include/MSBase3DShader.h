#pragma once
#include"MSBaseShader.h"
#include<memory>
//��DX11BaseShader

struct FBXMesh;
struct FBXModelData;
class DX11RenderResource;
class DXDisplay;
struct CBResource0;
class GameObjectBase;
class MSBase3DShader abstract :public MSBaseShader {
protected:
	MSBase3DShader(
		int constBufferSize0,
		int constBufferSize1,
		int constBufferSize2,
		unsigned int constVertexSize
	);
public:
	void Init()override;

	virtual void CustomRender(GameObjectBase*aObject);

	virtual void SetConstantBuffer0(CBResource0&aResource) = 0;
	//���b�V���P�ʂɗ^����
	//virtual void SetConstantBuffer1(DX11RenderResource * resource,DXDisplay*pDisplay) = 0;
	virtual void SetConstantBuffer1(FBXMesh& fbxMesh,
		DX11RenderResource* resource,
		DXDisplay* pDisplay) = 0;
	virtual void SetConstantBuffer2(std::weak_ptr<FBXModelData>modelData) = 0;

	MSVertexShader* GetVS();
	MSPixelShader* GetPS();

	ID3D11Buffer**GetCB0();
	ID3D11Buffer**GetCB1();
	ID3D11Buffer**GetCB2();
	const unsigned int GetVertexSize();
	//	friend void DX11Render::Render(DX11FbxManager*fbxManager, DX11RenderResource*resource);
protected:
	//���C�A�E�g�쐬
	//void InitLayout();

	ID3DBlob*mCompiledShader;
	ID3DBlob*mErrors;
	ID3D11Buffer*mConstantBuffer0;		//���b�V�����ɓK�p����o�b�t�@
	ID3D11Buffer*mConstantBuffer1;		//���b�V�����ɓK�p����o�b�t�@
	ID3D11Buffer*mConstantBuffer2;		//���b�V�����ɓK�p����o�b�t�@
protected:
	const int CONSTANTBUFFERSIZE0;
	const int CONSTANTBUFFERSIZE1;
	const int CONSTANTBUFFERSIZE2;
	const unsigned int VERTEXSIZE;			//��̒��_�̃T�C�Y

protected:
};
