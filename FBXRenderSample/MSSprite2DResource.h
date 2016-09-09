#pragma once
#include<memory>
#include"DX11TextureManager.h"
#include"DXVector2.h"
#include"DXMatrix.h"
//2D描画用
//using std::weak_ptr;
//using std::unique_ptr;
#include<D3DX11.h>
class MSSprite2DResource {
public:
	MSSprite2DResource();
	static void sInitialize(ID3D11Device*pDevice);
	//描画するテクスチャの設定
	void SetTexture(const std::weak_ptr<DX11TextureManager>&pTextureManager, const int pID);
	std::weak_ptr<DXTexture> GetTexture();
	void SetPosition(const DXVector2&pPosition);
	void SetPivot(const DXVector2&pPivot);
	void SetSize(const DXVector2&pSize);
	void SetScale(const DXVector2&pScale);
	const std::weak_ptr<DXVector2> GetPosition()const;
	const std::weak_ptr<DXVector2> GetPivot()const;
	const std::weak_ptr<DXVector2> GetSize()const;
	const std::weak_ptr<DXVector2> GetScale()const;
	const std::weak_ptr<DXMatrix>GetMatrix();
	//頂点バッファ
	ID3D11Buffer*mVertexBuffer;
private:
	//描画するテクスチャ
	std::weak_ptr<DXTexture> mTexture;
	
	std::shared_ptr<DXVector2>mPosition;	//描画位置
	std::shared_ptr<DXVector2> mPivot;		//中心となる位置
	std::shared_ptr<DXVector2> mSize;		//描画サイズ
	std::shared_ptr<DXVector2> mScale;		//拡大率
	std::shared_ptr<DXMatrix> mMatrix;		//位置、拡大率を合成する用の行列
	static ID3D11Device* sDevice;
};

