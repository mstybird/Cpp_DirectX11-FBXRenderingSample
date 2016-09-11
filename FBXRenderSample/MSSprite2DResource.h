#pragma once
#include<memory>
//#include"DXVector2.h"

//2D描画用
//using std::weak_ptr;
//using std::unique_ptr;
#include<D3DX11.h>
class DXVector2;
class DXVector3;
class DXMatrix;
class DXTexture;
class DX11TextureManager;
class DXDisplay;
struct SpriteVertex;
class MSSpriteBaseResource abstract{
public:
	MSSpriteBaseResource();
	static void sInitialize(ID3D11Device*pDevice);
	//描画するテクスチャの設定
	void SetTexture(const std::weak_ptr<DX11TextureManager>&pTextureManager, const int pID);
	std::weak_ptr<DXTexture> GetTexture();
	virtual void SetPosition(const DXVector2&pPosition);
	virtual void SetPosition(const DXVector3&pPosition);
	void SetPivot(const DXVector2&pPivot);
	void SetSize(const DXVector2&pSize);
	void SetScale(const DXVector2&pScale);
	const std::weak_ptr<DXVector3> GetPosition()const;
	const std::weak_ptr<DXVector2> GetPivot()const;
	const std::weak_ptr<DXVector2> GetSize()const;
	const std::weak_ptr<DXVector2> GetScale()const;
	const std::weak_ptr<DXMatrix>GetMatrix();
	std::unique_ptr<DXMatrix> GetMatrixWVP(const std::weak_ptr<DXDisplay>pDisplay);
	//頂点バッファ
	ID3D11Buffer*mVertexBuffer;
protected:
	//ピボット、サイズが変更された際にポリゴンを再生成する
	void CreateBuffer();
	virtual void CreatePolygon(SpriteVertex pPolygon[4]) = 0;;
	//描画するテクスチャ
	std::weak_ptr<DXTexture> mTexture;
	
	std::shared_ptr<DXVector3>mPosition;	//描画位置
	std::shared_ptr<DXVector2> mPivot;		//中心となる位置
	std::shared_ptr<DXVector2> mSize;		//描画サイズ
	std::shared_ptr<DXVector2> mScale;		//拡大率
	std::shared_ptr<DXMatrix> mMatrix;		//位置、拡大率を合成する用の行列
	static ID3D11Device* sDevice;
};

//スクリーン空間描画用
class MSSprite2DResource :public MSSpriteBaseResource {
public:
	void SetPosition(const DXVector2&pPosition)override;
	void SetPosition(const DXVector3&pPosition)override ;
private:
	void CreatePolygon(SpriteVertex pPolygon[4])override;

};
//3D空間描画用リソース
class MSSprite3DResource :public MSSpriteBaseResource {
public:
	void CreatePolygon(SpriteVertex pPolygon[4])override;

};