//ヘッダーファイルのインクルード
#include<stdio.h>
#include <windows.h>
#include<string>
//#include<map>
#include<unordered_map>
#include<vector>
#include<array>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>

#include<fbxsdk.h>
//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib,"shlwapi.lib")
//警告非表示
#pragma warning(disable : 4305)
//定数定義
#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ
#define APP_NAME "三角ポリゴン(最小シェーダー)"
//マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}



typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureName_ut;
//頂点の構造体
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //位置
};

struct SimpleIndex {
	unsigned int x, y, z;
};

struct ModelData {
	std::vector<SimpleVertex> Pos;
	std::vector<unsigned int> Index;

};


struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
};

class MYVBO {
public:
	struct SubMesh
	{
		SubMesh() : IndexOffset(0), TriangleCount(0) {}

		int IndexOffset;
		int TriangleCount;
	};
	MYVBO();
	~MYVBO();
	bool Initialize(const FbxMesh * pMesh);

	// Update vertex positions for deformed meshes.
	void UpdateVertexPosition(const FbxMesh * pMesh, const FbxVector4 * pVertices) const {};
	int GetSubMeshCount() const { return mSubMeshes.GetCount(); }
	FbxArray<FbxFloat>lVertices;	//頂点配列
	FbxArray<FbxUInt>lIndices;	//インデクス
	FbxArray<FbxFloat>lNormals;	//法線配列
	FbxArray<FbxFloat>lUVs;		//UV配列
	FbxArray<SubMesh*> mSubMeshes;

private:
	enum
	{
		VERTEX_VBO,
		NORMAL_VBO,
		UV_VBO,
		INDEX_VBO,
		VBO_COUNT,
	};

	// For every material, record the offsets in every VBO and triangle counts
	//VBOと三角系の個数、頂点インデックスオフセット

	FbxUInt mVBONames[VBO_COUNT];
	bool mHasNormal;			//法線を持っているかどうか
	bool mHasUV;				//UV座標を持っているかどうか
	bool mAllByControlPoint; // Save data in VBO by control point or by polygon vertex.

};

class MaterialCache {
public:
	MaterialCache();
	~MaterialCache();
	bool Initialize(const FbxSurfaceMaterial*pMaterial);
	static FbxDouble3 GetMaterialProperty(
		const FbxSurfaceMaterial*pMaterial,
		FbxString pPropertyName,
		FbxString pFactorPropertyName,
		FbxUInt&pTextureName
	);
	void SetCurrentMaterial()const {}
	bool HasTexture()const{}

	//初期色
	static void SetDefaultMaterial();
private:
	struct ColorChannel {
		ColorChannel() {
			Color[0] = 0.0f;
			Color[1] = 0.0f;
			Color[2] = 0.0f;
			Color[3] = 1.0f;
		}
		FbxUInt TextureName;
		FbxFloat Color[4];
	};
	ColorChannel Emissive;
	ColorChannel Ambient;
	ColorChannel Diffuse;
	ColorChannel Specular;
	FbxFloat Shinness;
};

class MYFBX {
public:

	void FbxInit(std::string vfileName);
	void FbxProc();
	void FbxDestroy();
	void FbxLoadFromFile();	//FBXファイル読み込み
	//シーンの読み込み(テクスチャ)
	void LoadCacheRecursive(FbxScene* pScene, FbxAnimLayer* pAnimLayer, std::string& pFbxFileName);
	//ノードの読み込み
	void LoadCacheRecursive(FbxNode*pNode, FbxAnimLayer*pAnimLayer);
	//シーンの解放
	void UnLoadCacheRecursive(FbxScene*pScene);
	//ノードの解放
	void UnLoadCacheRecursive(FbxNode*pNode);
	//デフォーマーの初期化
	void PreparePointCacheData(FbxScene*pScene, FbxTime&pCache_Start, FbxTime&pCache_Stop);
	//ポーズの種類を取得
	void FillPoseArray(FbxScene*pScene, FbxArray<FbxPose*>pPoseArray);
	TextureName_ut TextureFileName;


	//アニメーションの初期化
	void LoadAnimationData();
	//アニメーションデータがあるなら読み込んでおく
	void SetCurrentAnimStack();

	//頂点を取得する
	std::vector<std::vector<ModelData>> GetGeometryData();

	void GetNodeRecursive(FbxNode*pNode, FbxTime&pTime, FbxAnimLayer*pAnimLayer, FbxAMatrix&pParentGlobalPosition, FbxPose*pPose);

	//ノード一つの取得
	void GetNode(FbxNode*pNode, FbxTime&pTime, FbxAnimLayer*FbxAnimLayer, FbxAMatrix&pParentGlobalPosition, FbxAMatrix& pGlobalPosition, FbxPose*pPose);

	//ワールド座標を取得する
	FbxAMatrix GetGlobalPosition(FbxNode*pNode, const FbxTime&pTime, FbxPose*pPose, FbxAMatrix*pParentGlobalPosition=(fbxsdk::FbxAMatrix*)0);
	FbxAMatrix GetPoseMatrix(FbxPose*pPose, int pNodeIndex);

	//ジオメトリのオフセットを取得する
	FbxAMatrix GetGeometry(FbxNode*pNode);

	//アニメーション関係
	bool SetCurrentAnimStack(int pIndex);
	bool SetCurrentPoseIndex(int pPoseIndex);
	const FbxArray<FbxString*>&GetAnimStackNameArray()const { return AnimStackNameArray; }
//	void SetSelectNode(FbxNode*pSelectedNode);


private:

	//メンバ宣言部分
	std::string fileName;				//fbxファイル名
	FbxString WindowMessage;	//ウィンドウメッセージ用
	FbxManager * SdkManager;			//FBXマネージャ
	FbxScene * Scene;					//FBXシーン
	FbxImporter * Importer;			//FBXインポータ
	FbxAnimLayer * CurrentAnimLayer;	//FBXアニメーション
	FbxNode * SelectedNode;			//FBXノード


	int PoseIndex;								//ポーズのインデクス
	FbxArray<FbxString*> AnimStackNameArray;	//アニメーション名リスト
	FbxArray<FbxNode*> CameraArray;			//カメラリスト
	FbxArray<FbxPose*> PoseArray;				//ポーズのリスト

	FbxTime FrameTime, Start, Stop, CurrentTime;
	FbxTime Cache_Start, Cache_Stop;

	FbxUInt indexCount;

	std::vector<std::vector<ModelData>> Geometry;

};


//MAINクラス　定義
class MAIN
{

public:
	MAIN();
	~MAIN();
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPSTR);
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
	HRESULT InitD3D();
	HRESULT InitPolygon();
	HRESULT InitShader();
	void Loop();
	void App();
	void Render();
	void DestroyD3D();
	//↓アプリにひとつ
	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;
	ID3D11RasterizerState* m_pRasterizerState;
	//↓モデルの種類ごと(モデルの構造が全て同一ならアプリにひとつ）
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	//↓モデルごと
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer*m_pIndexBuffer;
	MYFBX fbx;
	std::vector<std::vector<ModelData>>vert;
};
