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
	virtual ~MSSpriteBaseResource();
	static void sInitialize(ID3D11Device*pDevice);
	//描画するテクスチャの設定
	void SetTexture(DX11TextureManager&pTextureManager, const int pID);
	void SetTexture(DXTexture*&pTexture);
	DXTexture* GetTexture();
	virtual void SetPosition(const DXVector2&pPosition);
	virtual void SetPosition(const DXVector3&pPosition);
	void SetRotation(const float aDegree);
	void AddRotation(const float aDegree);
	//ピボットを設定する
	void SetPivot(const DXVector2&pPivot);
	//ポリゴンサイズを設定する
	void SetSize(const DXVector2&pSize);
	//拡大率を設定する
	void SetScale(const DXVector2&pScale);
	//画像のサイズも含めて切り取る
	void SetSplitSize(const float pLeft, const float pRight, const float pTop, const float pBottom);
	void SetSplitSizeX(const DXVector2&pSizeX);
	void SetSplitSizeY(const DXVector2&pSizeY);

	//画像サイズはそのままで切り取る
	void SetSplitImage(const float pLeft, const float pRight, const float pTop, const float pBottom);

	//ポリゴン生成
	void CreateBuffer();

	const DXVector3* GetPosition()const;
	const DXVector2* GetPivot()const;
	const DXVector2* GetSize()const;
	const DXVector2* GetScale()const;
	DXMatrix* GetMatrix();
	void GetMatrixWVP(DXMatrix*pOut,DXDisplay& pDisplay);
	//頂点バッファの更新が必要か調べる
	bool IsUpdateBuffering();

	//頂点バッファ
	ID3D11Buffer*mVertexBuffer;
public:
	DXVector3 mPosition;	//描画位置
protected:
	//ポリゴン生成用
	virtual void CreatePolygon(SpriteVertex pPolygon[4]) = 0;
	//描画するテクスチャ
	DXTexture* mTexture;
	//このインスタンスがテクスチャの実態を保持しているかどうか
	
	DXVector2 mPivot;		//中心となる位置
	DXVector2 mSize;		//描画サイズ
	DXVector2 mScale;		//拡大率
	float mRotation;						//回転角度(ディグリー)
	DXVector2 mSplitPolygonX;				//ポリゴンの切り取り(水平)
	DXVector2 mSplitPolygonY;				//ポリゴンの切り取り(垂直)
	DXVector2 mSplitImageX;					//イマージの切り取り(水平)
	DXVector2 mSplitImageY;					//イマージの切り取り(垂直)
	DXMatrix mMatrix;		//位置、拡大率を合成する用の行列
	bool mUpdateFlag;	//ポリゴン再生成が必要な変更があった場合にtrueとなる
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