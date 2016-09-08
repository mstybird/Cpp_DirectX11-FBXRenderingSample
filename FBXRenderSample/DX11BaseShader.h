#pragma once
//警告非表示
#include<string>
#include<d3dx9.h>
#include<D3DX11.h>
#include<memory>
#include<vector>

using std::unique_ptr;
using std::vector;
#define ALIGN16 _declspec(align(16))

class DX11FbxManager;
struct FBXMesh;
struct FBXModelData;
class DX11RenderResource;
class DXDisplay;
class MSVertexShader;
class MSPixelShader;
class DX11SpriteResource;


//頂点シェーダークラス
class MSVertexShader {
public:
	static void sInitialize(ID3D11Device*pDevice, ID3D11DeviceContext*pDeviceContext);
	MSVertexShader();
	~MSVertexShader();
	//頂点シェーダの作成
	void Create(
		const std::string & fileName,
		const std::string & EntryPoint,
		const std::string & Version,
		vector<D3D11_INPUT_ELEMENT_DESC>&pLayout
		);
	ID3D11VertexShader* GetShader()const;
	ID3D11InputLayout* GetLayout()const;
	void SetShader();
private:
	ID3D11VertexShader*mVertexShader;
	ID3D11InputLayout*mVertexLayout;
	ID3DBlob*mCompiledShader;
	ID3DBlob*mErrors;

	static ID3D11Device*sDevice;
	static ID3D11DeviceContext*sDeviceContext;
};

//ピクセルシェーダークラス
class MSPixelShader {
public:
	static void sInitialize(ID3D11Device*pDevic, ID3D11DeviceContext*pDeviceContext);
	MSPixelShader();
	~MSPixelShader();
	void Create(const std::string & pFileName,
		const std::string & pEntryPoint,
		const std::string & pVersion);
	ID3D11PixelShader* GetShader()const;
	void SetShader();

private:
	ID3D11PixelShader*mPixelShader;
	ID3DBlob*mCompiledShader;
	ID3DBlob*mErrors;
	static ID3D11Device*sDevice;
	static ID3D11DeviceContext*sDeviceContext;
};

class DX11BaseShader {
protected:
	DX11BaseShader(
		int constBufferSize1, 
		int constBufferSize2,
		unsigned int constVertexSize
		);
public:

	static void Init(ID3D11Device*pDevice, ID3D11DeviceContext*pDeviceContext);
	void Init();
	void InitVertex(
		const std::string&fileName,
		const std::string&EntryPoint = "VS",
		const std::string&Version = "vs_5_0"
	);
	void InitPixel(
		const std::string&fileName,
		const std::string&EntryPointp = "PS",
		const std::string&Version = "ps_5_0"
	);

	//メッシュ単位に与える
	//virtual void SetConstantBuffer1(DX11RenderResource * resource,DXDisplay*pDisplay) = 0;
	virtual void SetConstantBuffer1(FBXMesh * fbxMesh, DX11RenderResource * resource, DXDisplay*pDisplay) = 0;
	virtual void SetConstantBuffer2(FBXModelData*modelData) = 0;

	MSVertexShader* GetVS();
	MSPixelShader* GetPS();
	ID3D11Buffer**GetCB1();
	ID3D11Buffer**GetCB2();
	const unsigned int GetVertexSize();
//	friend void DX11Render::Render(DX11FbxManager*fbxManager, DX11RenderResource*resource);
protected:
	//レイアウト作成
	//void InitLayout();
	virtual void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout) = 0;
	MSVertexShader mVertexShader;
	MSPixelShader mPixelShader;
	
	ID3DBlob*mCompiledShader;
	ID3DBlob*mErrors;
	ID3D11Buffer*mConstantBuffer1;		//一度のみ適用するバッファ
	ID3D11Buffer*mConstantBuffer2;		//メッシュ毎に適用するバッファ
protected:
	const int CONSTANTBUFFERSIZE1;
	const int CONSTANTBUFFERSIZE2;	
	const unsigned int VERTEXSIZE;			//一つの頂点のサイズ

protected:
	static ID3D11Device*sDevice;				//DirectX11デバイス
	static ID3D11DeviceContext*sDeviceContext;	//DirectX11デバイスコンテキスト
};

//class DX11SpriteResource;

class DX11BaseSprite {
public:

	struct CONSTANTDATA {
		ALIGN16 D3DXMATRIX mW;
	};

	struct SpriteVertex
	{
		D3DXVECTOR3 Pos; //位置
		D3DXVECTOR2 UV; //テクスチャー座標
	};

	DX11BaseSprite();
	~DX11BaseSprite();
	static void Init(ID3D11Device*pDevice, ID3D11DeviceContext*pDeviceContext);
	void Init();
	void InitVertex(
		const std::string&fileName,
		const std::string&EntryPoint = "VS",
		const std::string&Version = "vs_5_0"
		);
	void InitPixel(
		const std::string&fileName,
		const std::string&EntryPointp = "PS",
		const std::string&Version = "ps_5_0"
		);


	void SetConstantBuffer(DX11SpriteResource&pResource);

	
	void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout);
	
	
	
	
	MSVertexShader mVertexShader;
	MSPixelShader mPixelShader;



protected:
	ID3D11Buffer*mConstantBuffer;

	const int CONSTANTBUFFERSIZE1;
	static ID3D11Device*sDevice;				//DirectX11デバイス
	static ID3D11DeviceContext*sDeviceContext;	//DirectX11デバイスコンテキスト

};
