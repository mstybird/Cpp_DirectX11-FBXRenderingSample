#pragma once
#include<memory>
#include"DXMath.hpp"

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
	void SetTexture(DX11TextureManager&pTextureManager, const int pID, const bool IsCreateBuffer);
	void SetTexture(DXTexture*&pTexture, const bool IsCreateBuffer);
	DXTexture* GetTexture();
	virtual void SetPosition(const DXVector2&pPosition);
	virtual void SetPosition(const DXVector3&pPosition);
	//ピボットを設定する
	void SetPivot(const DXVector2&pPivot, const bool IsCreateBuffer);
	//ポリゴンサイズを設定する
	void SetSize(const DXVector2&pSize, const bool IsCreateBuffer);
	//拡大率を設定する
	void SetScale(const DXVector2&pScale);
	//画像のサイズも含めて切り取る
	void SetSplitSize(const float pLeft, const float pRight, const float pTop, const float pBottom, const bool IsCreateBuffer);
	void SetSplitSizeX(const DXVector2&pSizeX, const bool IsCreateBuffer);
	void SetSplitSizeY(const DXVector2&pSizeY, const bool IsCreateBuffer);

	//画像サイズはそのままで切り取る
	void SetSplitImage(const float pLeft, const float pRight, const float pTop, const float pBottom, const bool IsCreateBuffer);

	//ポリゴン生成
	void CreateBuffer();

	const std::weak_ptr<DXVector3> GetPosition()const;
	const std::weak_ptr<DXVector2> GetPivot()const;
	const std::weak_ptr<DXVector2> GetSize()const;
	const std::weak_ptr<DXVector2> GetScale()const;
	const std::weak_ptr<DXMatrix>GetMatrix();
	DXMatrix GetMatrixWVP(DXDisplay& pDisplay);
	//頂点バッファ
	ID3D11Buffer*mVertexBuffer;
protected:
	//ポリゴン生成用
	virtual void CreatePolygon(SpriteVertex pPolygon[4]) = 0;
	//描画するテクスチャ
	DXTexture* mTexture;
	//このインスタンスがテクスチャの実態を保持しているかどうか
	
	std::shared_ptr<DXVector3>mPosition;	//描画位置
	std::shared_ptr<DXVector2> mPivot;		//中心となる位置
	std::shared_ptr<DXVector2> mSize;		//描画サイズ
	std::shared_ptr<DXVector2> mScale;		//拡大率
	DXVector2 mSplitPolygonX;				//ポリゴンの切り取り(水平)
	DXVector2 mSplitPolygonY;				//ポリゴンの切り取り(垂直)
	DXVector2 mSplitImageX;					//イマージの切り取り(水平)
	DXVector2 mSplitImageY;					//イマージの切り取り(垂直)
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