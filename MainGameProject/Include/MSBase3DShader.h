#pragma once
#include"MSBaseShader.h"
#include<memory>
//元DX11BaseShader

struct FBXMesh;
struct FBXModelData;
class DX11RenderResource;
class DXDisplay;
class MSBase3DShader abstract :public MSBaseShader {
protected:
	MSBase3DShader(
		int constBufferSize1,
		int constBufferSize2,
		unsigned int constVertexSize
	);
public:
	void Init()override;


	//メッシュ単位に与える
	//virtual void SetConstantBuffer1(DX11RenderResource * resource,DXDisplay*pDisplay) = 0;
	virtual void SetConstantBuffer1(FBXMesh& fbxMesh,
		DX11RenderResource& resource,
		DXDisplay& pDisplay) = 0;
	virtual void SetConstantBuffer2(std::weak_ptr<FBXModelData>modelData) = 0;

	MSVertexShader* GetVS();
	MSPixelShader* GetPS();
	ID3D11Buffer**GetCB1();
	ID3D11Buffer**GetCB2();
	const unsigned int GetVertexSize();
	//	friend void DX11Render::Render(DX11FbxManager*fbxManager, DX11RenderResource*resource);
protected:
	//レイアウト作成
	//void InitLayout();

	ID3DBlob*mCompiledShader;
	ID3DBlob*mErrors;
	ID3D11Buffer*mConstantBuffer1;		//一度のみ適用するバッファ
	ID3D11Buffer*mConstantBuffer2;		//メッシュ毎に適用するバッファ
protected:
	const int CONSTANTBUFFERSIZE1;
	const int CONSTANTBUFFERSIZE2;
	const unsigned int VERTEXSIZE;			//一つの頂点のサイズ

protected:
};
