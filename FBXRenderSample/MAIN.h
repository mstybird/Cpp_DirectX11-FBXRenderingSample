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

#ifdef _DEBUG
#pragma comment(lib,"libfbxsdk-md.lib")
#else
#pragma comment(lib,"libfbxsdk-md.lib")
#endif
#pragma comment(lib,"shlwapi.lib")
//�x����\��
#pragma warning(disable : 4305)
//�萔��`
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����
#define APP_NAME "�O�p�|���S��(�ŏ��V�F�[�_�[)"
//�}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

#define _CRTDBG_MAP_ALLOC #include <stdlib.h> #include <crtdbg.h>

typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureName_ut;
//���_�̍\����
struct SimpleVector4 {
	float x, y, z, w;
	SimpleVector4() {
		w = 1.0f;
	}
};

struct SimpleVector3 {
	float x, y, z;

};

struct SimpleVector2 {
	float x, y;

};


struct SimpleVertex
{
	D3DXVECTOR4 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};


struct SimpleIndex {
	unsigned int x, y, z;
};


struct FBXModelData {

	std::vector<SimpleVertex> Data;

	//SimpleVector4*Pos;


	unsigned int PosLength;
	unsigned int *Index;
	unsigned int IndexLength;
};


struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mW;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	D3DXMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	D3DXVECTOR4 LightDir;
	D3DXVECTOR4 Diffuse;
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
	void UpdateVertexPosition(const FbxMesh * pMesh, const FbxVector4 * pVertices);
	int GetSubMeshCount() const { return mSubMeshes.GetCount(); }

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
private:
	//���[�J�������o�֐��p
	struct T_LocalComputeClusterDeformation {
		FbxAMatrix lReferenceGlobalInitPosition;	//�����ʒu
		FbxAMatrix lReferenceGlobalCurrentPosition;	//���݈ʒu
		FbxAMatrix lAssociateGlobalInitPosition;	//�����ʒu
		FbxAMatrix lAssociateGlobalCurrentPosition;	//���݈ʒu
		FbxAMatrix lClusterGlobalInitPosition;		//�����ʒu
		FbxAMatrix lClusterGlobalCurrentPosition;	//���݈ʒu

		FbxAMatrix lReferenceGeometry;
		FbxAMatrix lAssociateGeometry;
		FbxAMatrix lClusterGeometry;

		FbxAMatrix lClusterRelativeInitPosition;
		FbxAMatrix lClusterRelativeCurrentPositionInverse;
	};

	struct T_LocalGetPoseMatrix {
		FbxAMatrix lPoseMatrix;
		FbxMatrix lMatrix;
	};

	struct T_LocalGetNodeRecursive {
		FbxAMatrix lGlobalPosition;
		FbxAMatrix lGeometryOffset;
		FbxAMatrix lGlobalOffPosition;
	};

public:
	//��������p
	~MYFBX();

	DWORD start, end;
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
	std::vector<std::vector<FBXModelData*>>* GetGeometryData();

	void GetNodeRecursive(FbxNode*pNode, FbxTime&pTime, FbxAnimLayer*pAnimLayer, FbxAMatrix&pParentGlobalPosition, FbxPose*pPose);

	//�m�[�h��̎擾
	void GetNode(FbxNode*pNode, FbxTime&pTime, FbxAnimLayer*FbxAnimLayer, FbxAMatrix&pParentGlobalPosition, FbxAMatrix& pGlobalPosition, FbxPose*pPose);

	//���[���h���W���擾����
	void GetGlobalPosition(FbxAMatrix&pDstMatrix,FbxNode*pNode, const FbxTime&pTime, FbxPose*pPose, FbxAMatrix*pParentGlobalPosition=(fbxsdk::FbxAMatrix*)0);
	FbxAMatrix GetPoseMatrix(FbxPose*pPose, int pNodeIndex);

	//�W�I���g���̃I�t�Z�b�g���擾����
	void GetGeometry(FbxAMatrix&pSrcMatrix,FbxNode*pNode);

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
	
	void ComputeClusterDeformation(FbxAMatrix&pGlobalPosition, FbxMesh*pMesh,FbxCluster*pCluster,FbxAMatrix& pVertexTransformMatrix,  FbxTime pTime, FbxPose*pPose);

	void MatrixScale(FbxDouble*pMatrix, double pValue);
	void MatrixAddToDiagonal(FbxAMatrix&pMatrix, double pValue);
	void MatrixAdd(FbxDouble*pDstMatrix, FbxDouble*pSrcMatrix);

private:
	//���\�b�h
	void MatrixInverse(FbxDouble*pDstMatrix);
	void MatrixFbxToD3DX(D3DXMATRIX*pDstMatrix, FbxDouble*pSrcMatrix);
	void MatrixD3DXToFbx(FbxDouble*pDstMatrix, D3DXMATRIX*pSrcMatrix);
	//FBX���f���f�[�^�̉��
	void ReleaseGeometryData();
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

	std::vector<std::vector<FBXModelData*>> Geometry;

	//�����o�֐��p���[�J���ϐ��Q
	//T_LocalComputeLinearDeformation tl_CLD;
	T_LocalComputeClusterDeformation tlCCD;
	T_LocalGetPoseMatrix tlGM;
	T_LocalGetNodeRecursive tlGNR;
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
	std::vector<std::vector<FBXModelData*>>*vert;
};
