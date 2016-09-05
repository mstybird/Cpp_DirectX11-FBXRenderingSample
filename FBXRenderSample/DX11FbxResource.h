#pragma once
#include<d3dx9.h>
#include<vector>
#include"DX11Texture.h"
struct ColorChannel {
	ColorChannel();
	~ColorChannel();
	void CreateTexture();
	std::string TextureName;	//テクスチャファイルパス
	DXTexture*mTexture;			//テクスチャリソース
	float Color[4];				//マテリアルカラー
};

struct SimpleVertex
{
	D3DXVECTOR4 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};
constexpr int GEOMETRYSIZE = sizeof(SimpleVertex);

struct FBXModelData {

	FBXModelData();
	~FBXModelData();
	std::vector<SimpleVertex> Data;	//ジオメトリデータ

	unsigned int PosLength;		//頂点数
	unsigned int *Index;		//インデックスバッファ生成用
	unsigned int IndexLength;	//インデックス数

	ColorChannel *Emissive;		//エミッシブ
	ColorChannel *Ambient;		//アンビエント
	ColorChannel *Diffuse;		//ディフューズ
	ColorChannel *Specular;		//スペキュラ



};
/*
	mesh:
		sub
		sub
	mesh:
		sub
	vector<FBXMesh>data;
	data[0].modeldata[0]

*/
//メッシュ一つの情報
struct FBXMesh {
	FBXMesh();
	~FBXMesh();
	//メッシュ一つに含まれるサブメッシュの配列
	std::vector<FBXModelData*>subMesh;
	//メッシュの行列
	D3DXMATRIX *mWorld;
};
/*
	タスク：
	メッシュとサブメッシュの区別
	メッシュのグローバルポジションの適用
	HLSLにギュローバルポジションを適用する
	メモリ最適化
	外部からアニメーションの設定
*/