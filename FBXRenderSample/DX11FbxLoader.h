#pragma once
//�x����\��

#include<fbxsdk.h>
#include<string>
#include<vector>
#include<D3DX9.h>
#ifdef _DEBUG
#pragma comment(lib,"libfbxsdk-md.lib")
#else
#pragma comment(lib,"libfbxsdk-md.lib")
#endif
#include<unordered_map>
typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureName_ut;

#include"DX11FbxResource.h"


#include"FbxVBO.h"
#include"FbxMaterialCache.h"
#include"FbxComputeDeformer.h"

class DX11FbxLoader {

public:
	//��������p
	~DX11FbxLoader();
	void FbxInit(std::string vfileName, bool animationLoad = true);
	//�A�j���[�V�����f�[�^�̂ݕʂœǂݍ���
	//�쐬�r��
	void FbxLoadAnimationFromFile(std::string vfileName);
	void LoadAnimationRecursive(FbxScene* pScene, FbxAnimLayer* pAnimLayer);
	void LoadAnimationRecursive(FbxNode*pNode, FbxAnimLayer*pAnimLayer);
	//FBX�N���X���
	void FbxDestroy();
	//���f���f�[�^���擾����
	std::vector<std::vector<FBXModelData*>>* GetGeometryData2(D3DXVECTOR3 *transPos);
	//�A�j���[�V�����̐؂�ւ�
	void SetAnimation(std::string pName);
	void SetAnimation(int pIndex);

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
	FbxAnimLayer * CurrentAnimLayer;	//FBX�A�j���[�V����
	FbxTime FrameTime, Start, Stop, CurrentTime;


	std::unordered_map<std::string, int>AnimData;		//�A�j���[�V�����X�^�b�N
	std::vector<std::vector<FBXModelData*>> Geometry;	//���b�V���f�[�^
	std::vector<FbxNode*>nodemeshes;	//���b�V���������m�[�h���X�g
	std::vector<FbxNode*>nodeAnimeMeshes;	//���b�V���������m�[�h���X�g(�A�j���[�V����)
};