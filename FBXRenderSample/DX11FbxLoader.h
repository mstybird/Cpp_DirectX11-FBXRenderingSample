#pragma once
//�x����\��
#pragma warning(disable : 4005)
#include<fbxsdk.h>
#include<string>
#include<vector>
#include<D3DX10.h>
#ifdef _DEBUG
#pragma comment(lib,"libfbxsdk-md.lib")
#else
#pragma comment(lib,"libfbxsdk-md.lib")
#endif
#include<unordered_map>
typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureName_ut;



struct ColorChannel {
	ColorChannel() {
		Color[0] = 0.0f;
		Color[1] = 0.0f;
		Color[2] = 0.0f;
		Color[3] = 1.0f;
	}
	FbxString TextureName;
	FbxFloat Color[4];
};

struct SimpleVertex
{
	D3DXVECTOR4 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};
constexpr int GEOMETRYSIZE = sizeof(SimpleVertex);

struct FBXModelData {

	std::vector<SimpleVertex> Data;	//�W�I���g���f�[�^

	unsigned int PosLength;
	unsigned int *Index;		//�C���f�b�N�X�o�b�t�@�����p
	unsigned int IndexLength;	//�C���f�b�N�X��

	ColorChannel *Emissive;		//�G�~�b�V�u
	ColorChannel *Ambient;		//�A���r�G���g
	ColorChannel *Diffuse;		//�f�B�t���[�Y
	ColorChannel *Specular;		//�X�y�L����



};


#include"FbxVBO.h"
#include"FbxMaterialCache.h"
#include"FbxComputeDeformer.h"

class DX11FbxLoader {

public:
	//��������p
	~DX11FbxLoader();
	void FbxInit(std::string vfileName);
	//FBX�N���X���
	void FbxDestroy();
	//���f���f�[�^���擾����
	std::vector<std::vector<FBXModelData*>>* GetGeometryData2(D3DXVECTOR3 *transPos);
private:
	//FBX�t�@�C���ǂݍ���
	void FbxLoadFromFile();	
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


	//�A�j���[�V�����̏�����
	void LoadAnimationData();





	//�A�j���[�V�����֌W
	bool SetCurrentAnimStack(int pIndex);
	bool SetCurrentPoseIndex(int pPoseIndex);
	const FbxArray<FbxString*>&GetAnimStackNameArray()const { return AnimStackNameArray; }
	//	void SetSelectNode(FbxNode*pSelectedNode);

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

	TextureName_ut TextureFileName;

	int PoseIndex;								//�|�[�Y�̃C���f�N�X
	FbxArray<FbxString*> AnimStackNameArray;	//�A�j���[�V���������X�g
	FbxArray<FbxPose*> PoseArray;				//�|�[�Y�̃��X�g

	FbxTime FrameTime, Start, Stop, CurrentTime;
	FbxTime Cache_Start, Cache_Stop;

	FbxUInt indexCount;

	std::vector<std::vector<FBXModelData*>> Geometry;
	std::vector<FbxNode*>nodemeshes;
};