//�w�b�_�[�t�@�C���̃C���N���[�h
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
//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib,"shlwapi.lib")
//�x����\��
#pragma warning(disable : 4305)
//�萔��`
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����
#define APP_NAME "�O�p�|���S��(�ŏ��V�F�[�_�[)"
//�}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}



typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureName_ut;
//���_�̍\����
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //�ʒu
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
	D3DXMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
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
	void UpdateVertexPosition(FbxMesh*pMesh, FbxVector4*pVertexArray);

	FbxArray<FbxFloat>lVertices;	//���_�z��
	FbxArray<FbxUInt>lIndices;	//�C���f�N�X
	FbxArray<FbxFloat>lNormals;	//�@���z��
	FbxArray<FbxFloat>lUVs;		//UV�z��
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
	//VBO�ƎO�p�n�̌��A���_�C���f�b�N�X�I�t�Z�b�g

	FbxUInt mVBONames[VBO_COUNT];
	bool mHasNormal;			//�@���������Ă��邩�ǂ���
	bool mHasUV;				//UV���W�������Ă��邩�ǂ���
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

	//�����F
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
	void FbxLoadFromFile();	//FBX�t�@�C���ǂݍ���
	//�V�[���̓ǂݍ���(�e�N�X�`��)
	void LoadCacheRecursive(FbxScene* pScene, FbxAnimLayer* pAnimLayer, std::string& pFbxFileName);
	//�m�[�h�̓ǂݍ���
	void LoadCacheRecursive(FbxNode*pNode, FbxAnimLayer*pAnimLayer);
	//�V�[���̉��
	void UnLoadCacheRecursive(FbxScene*pScene);
	//�m�[�h�̉��
	void UnLoadCacheRecursive(FbxNode*pNode);
	//�f�t�H�[�}�[�̏�����
	void PreparePointCacheData(FbxScene*pScene, FbxTime&pCache_Start, FbxTime&pCache_Stop);
	//�|�[�Y�̎�ނ��擾
	void FillPoseArray(FbxScene*pScene, FbxArray<FbxPose*>pPoseArray);
	TextureName_ut TextureFileName;


	//�A�j���[�V�����̏�����
	void LoadAnimationData();

	//���_���擾����
	std::vector<std::vector<ModelData>> GetGeometryData();

	void GetNodeRecursive(FbxNode*pNode, FbxTime&pTime, FbxAnimLayer*pAnimLayer, FbxAMatrix&pParentGlobalPosition, FbxPose*pPose);

	//�m�[�h��̎擾
	void GetNode(FbxNode*pNode, FbxTime&pTime, FbxAnimLayer*FbxAnimLayer, FbxAMatrix&pParentGlobalPosition, FbxAMatrix& pGlobalPosition, FbxPose*pPose);

	//���[���h���W���擾����
	FbxAMatrix GetGlobalPosition(FbxNode*pNode, const FbxTime&pTime, FbxPose*pPose, FbxAMatrix*pParentGlobalPosition=(fbxsdk::FbxAMatrix*)0);
	FbxAMatrix GetPoseMatrix(FbxPose*pPose, int pNodeIndex);

	//�W�I���g���̃I�t�Z�b�g���擾����
	FbxAMatrix GetGeometry(FbxNode*pNode);

	//�A�j���[�V�����֌W
	bool SetCurrentAnimStack(int pIndex);
	bool SetCurrentPoseIndex(int pPoseIndex);
	const FbxArray<FbxString*>&GetAnimStackNameArray()const { return AnimStackNameArray; }
//	void SetSelectNode(FbxNode*pSelectedNode);

	//�f�t�H�[�}�[���A�j���[�V���������֌W
	void ReadVertexCacheData(FbxMesh*pMesh, FbxTime&pTime, FbxVector4*pVertexArray);

	void ComputeShapeDeformation(FbxMesh*pMesh, FbxTime&pTime, FbxAnimLayer*pAnimLayer, FbxVector4*pVertexArray);
	void ComputeSkinDeformation(FbxAMatrix&pGlobalPosition, FbxMesh*pMesh, FbxTime&pTime, FbxVector4*pVertexArray, FbxPose*pPose);
	void ComputeLinearDeformation(FbxAMatrix&pGlobalPosition, FbxMesh*pMesh, FbxTime&pTime, FbxVector4*pVertexArray, FbxPose*pPose);
	void ComputeDualQuaternionDeformation(FbxAMatrix&pGlobalPosition, FbxMesh*pMesh, FbxTime&pTime, FbxVector4*pVertexArray, FbxPose*pPose);

private:

	//�����o�錾����
	std::string fileName;				//fbx�t�@�C����
	FbxString WindowMessage;	//�E�B���h�E���b�Z�[�W�p
	FbxManager * SdkManager;			//FBX�}�l�[�W��
	FbxScene * Scene;					//FBX�V�[��
	FbxImporter * Importer;			//FBX�C���|�[�^
	FbxAnimLayer * CurrentAnimLayer;	//FBX�A�j���[�V����
	FbxNode * SelectedNode;			//FBX�m�[�h


	int PoseIndex;								//�|�[�Y�̃C���f�N�X
	FbxArray<FbxString*> AnimStackNameArray;	//�A�j���[�V���������X�g
	FbxArray<FbxNode*> CameraArray;			//�J�������X�g
	FbxArray<FbxPose*> PoseArray;				//�|�[�Y�̃��X�g

	FbxTime FrameTime, Start, Stop, CurrentTime;
	FbxTime Cache_Start, Cache_Stop;

	FbxUInt indexCount;

	std::vector<std::vector<ModelData>> Geometry;

};


//MAIN�N���X�@��`
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
	//���A�v���ɂЂƂ�
	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;
	ID3D11RasterizerState* m_pRasterizerState;
	//�����f���̎�ނ���(���f���̍\�����S�ē���Ȃ�A�v���ɂЂƂj
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	//�����f������
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer*m_pIndexBuffer;
	MYFBX fbx;
	std::vector<std::vector<ModelData>>vert;
};
