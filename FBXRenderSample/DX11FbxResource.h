#pragma once
#include<d3dx9.h>
#include<vector>
#include<memory>
#include<array>
#include"DXVector2.h"
#include"DXVector3.h"
class DXTexture;
class DX11TextureManager;

struct ColorChannel {
	ColorChannel();
	~ColorChannel();
	void CreateTexture();
	std::string TextureName;	//テクスチャファイルパス
//	DXTexture*mTexture;			//テクスチャリソース
	std::weak_ptr<DXTexture>mTexture;
	int mTextureID;
	float Color[4];				//マテリアルカラー


	static std::unique_ptr<DX11TextureManager>sTextureManager;
	static int sTextureCounter;
};

struct FbxVertex
{
	DXVector3 Pos;
	DXVector3 Normal;
	DXVector2 UV;
};
constexpr int GEOMETRYSIZE = sizeof(FbxVertex);

struct FBXModelData {

	FBXModelData();
	~FBXModelData();
	std::vector<FbxVertex> Data;	//ジオメトリデータ

	//三角形ポリゴン一つを形成するジオメトリ3つを取得する
	void GetPolygon(std::array<FbxVertex*, 3>&pOutPolygon,unsigned int pPolygonIndex);

	unsigned int PolygonCount;

	unsigned int PosLength;		//頂点数
	unsigned int *Index;		//インデックスバッファ生成用
	unsigned int IndexLength;	//インデックス数

	
	std::weak_ptr<ColorChannel> Emissive;		//エミッシブ
	std::weak_ptr<ColorChannel> Ambient;		//アンビエント
	std::weak_ptr<ColorChannel> Diffuse;		//ディフューズ
	std::weak_ptr<ColorChannel> Specular;		//スペキュラ



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
	std::vector<std::shared_ptr<FBXModelData>>subMesh;
	//メッシュの行列
	std::shared_ptr<D3DXMATRIX>mWorld;
//	D3DXMATRIX *mWorld;
};
/*
	タスク：
	メッシュとサブメッシュの区別
	メッシュのグローバルポジションの適用
	HLSLにギュローバルポジションを適用する
	メモリ最適化
	外部からアニメーションの設定
*/