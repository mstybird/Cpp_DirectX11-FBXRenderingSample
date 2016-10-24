#pragma once
//�x����\��

#include<fbxsdk.h>
#include<string>
#include<vector>
#include<D3DX9.h>
#ifdef _DEBUG
#pragma comment(lib,"libfbxsdk-mt.lib")
#else
#pragma comment(lib,"libfbxsdk-md.lib")
#endif
#include<unordered_map>
#include<memory>
typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureName_ut;
//using MeshVector = std::vector<std::shared_ptr<FBXMesh>>;
#include"DX11FbxResource.h"


#include"FbxVBO.h"
#include"FbxMaterialCache.h"
#include"FbxComputeDeformer.h"
class MSFbxObject;
class DX11FbxLoader {
	friend class MSFbxObject;
public:
	//��������p
	DX11FbxLoader();
	~DX11FbxLoader();
	void FbxInit(std::string vfileName, bool animationLoad = true);
	//�A�j���[�V�����f�[�^�̂ݕʂœǂݍ���
	//�쐬�r��
	void FbxLoadAnimationFromFile(std::string vfileName);
	void LoadAnimationRecursive(FbxScene* pScene, FbxAnimLayer* pAnimLayer);
	void LoadAnimationRecursive(FbxNode*pNode, FbxAnimLayer*pAnimLayer);
	//FBX�N���X���
	void FbxDestroy();
	//���b�V���f�[�^���擾����
	std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>> GetGeometryData2(FbxTime&aCurrentTime);
	void GetMeshCount(std::vector<int>&pCountVector);
	void GetMeshVertexCount(std::vector<std::vector<int>>&pCountVector);
	void GetMeshIndexCount(std::vector<std::vector<int>>&pCountVector);
	//�A�j���[�V�����̐؂�ւ�
	void SetAnimation(std::string pName);
	void SetAnimation(int pIndex);

	FbxTime GetStartFrame() {
		return Start;
	}
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
	//�A�j���[�V�����̃Z�b�g
	bool SetCurrentAnimStack(int pIndex);
	bool SetCurrentPoseIndex(int pPoseIndex);
	const FbxArray<FbxString*>&GetAnimStackNameArray()const { return AnimStackNameArray; }
	//	void SetSelectNode(FbxNode*pSelectedNode);

	void ReleaseGeometryData();



private:

	//�����o�錾����
	std::string fileName;				//fbx�t�@�C����
	std::string fileRelativePath;		//FBX�t�@�C���̂���t�H���_�ւ̑��΃p�X

	FbxString WindowMessage;	//�E�B���h�E���b�Z�[�W�p
	FbxManager * SdkManager;			//FBX�}�l�[�W��
	FbxScene * Scene;					//FBX�V�[��
	FbxScene * SceneAnim;				//FBX�V�[��(�A�j���[�V�����p)
	FbxImporter * Importer;			//FBX�C���|�[�^
	FbxNode * SelectedNode;			//FBX�m�[�h

	//TextureName_ut TextureFileName;

	int PoseIndex;								//�|�[�Y�̃C���f�N�X
	FbxArray<FbxString*> AnimStackNameArray;	//�A�j���[�V���������X�g
	FbxArray<FbxPose*> PoseArray;				//�|�[�Y�̃��X�g
	FbxTime Cache_Start, Cache_Stop;

	//�A�j���[�V�������
	bool EnableAnimation;	//�A�j���[�V�������L�����ǂ���
	FbxAnimLayer * CurrentAnimLayer;	//FBX�A�j���[�V����
	FbxTime FrameTime, Start, Stop;


	std::unordered_map<std::string, int>AnimData;		//�A�j���[�V�����X�^�b�N
	//std::vector<std::vector<FBXModelData*>> Geometry;	//���b�V���f�[�^
	std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>>mMesh;	//�T�u���b�V�����܂ރ��b�V���f�[�^
	std::vector<FbxNode*>nodemeshes;	//���b�V���������m�[�h���X�g
	std::vector<FbxNode*>nodeAnimeMeshes;	//���b�V���������m�[�h���X�g(�A�j���[�V����)
};