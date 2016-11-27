#pragma once
//�x����\��

#include<fbxsdk.h>

#include<string>
#include<vector>


#include<unordered_map>
#include<memory>
typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureName_ut;
#include"DX11FbxResource.h"


#include"FbxVBO.h"
#include"FbxMaterialCache.h"
#include"FbxComputeDeformer.h"

struct FbxDataCache {

	FbxDataCache()
	{
		
	};

	~FbxDataCache() {


		auto DeleteVector = [](auto&aVector) {
			for (auto&v : aVector) {
				FBX_SAFE_DELETE_ARRAY(v);
			}
		};

		DeleteVector(dstVertexArray);
		DeleteVector(vertexArrayLinear);
		DeleteVector(vertexArrayDQ);
		DeleteVector(clusterDeformation);
		DeleteVector(clusterLinearWeight);
		DeleteVector(dQClusterDeformation);
		DeleteVector(clusterDualWeight);
		DeleteVector(vertexArray);


	}

	void FirstReserve(const int aLength) {
		if (dstVertexArray.size() == 0) {
			dstVertexArray.resize(aLength);
		}
		if (vertexArrayLinear.size() == 0) {
			vertexArrayLinear.resize(aLength);
		}
		if (vertexArrayDQ.size() == 0) {
			vertexArrayDQ.resize(aLength);
		}
		if (clusterDeformation.size() == 0) {
			clusterDeformation.resize(aLength);
		}
		if (clusterLinearWeight.size() == 0) {
			clusterLinearWeight.resize(aLength);
		}
		if (dQClusterDeformation.size() == 0) {
			dQClusterDeformation.resize(aLength);
		}
		if (clusterDualWeight.size() == 0) {
			clusterDualWeight.resize(aLength);
		}
		if (vertexArray.size() == 0) {
			vertexArray.resize(aLength);
		}
	}

	std::vector<FbxVector4*>dstVertexArray;
	std::vector<FbxVector4*>vertexArrayLinear;
	std::vector<FbxVector4*>vertexArrayDQ;
	std::vector<FbxAMatrix*>clusterDeformation;

	std::vector<FbxDualQuaternion*>dQClusterDeformation;
	std::vector<double*>clusterLinearWeight;
	std::vector<double*>clusterDualWeight;

	std::vector<FbxVector4*> vertexArray;
};


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
	int GetDefaultAnimation() {
		return mDefaultAnimation;
	}

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

	/*********�A�j���[�V�����v�Z�֌W*********/

	void ReadVertexCacheData(FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray);
	void ComputeShapeDeformation(const int pIndex, FbxMesh * pMesh, FbxTime & pTime, FbxAnimLayer * pAnimLayer, FbxVector4 * pVertexArray);
	//�X�L���f�t�H�[�}�̌v�Z
	void ComputeSkinDeformation(const int pIndex, FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose);
	void ComputeLinearDeformation(const int pIndex, FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose);
	void ComputeDualQuaternionDeformation(const int pIndex, FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose);
	void ComputeClusterDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix, FbxTime pTime, FbxPose* pPose);
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
	int mDefaultAnimation;	//�f�t�H���g�A�j���[�V����

	std::unordered_map<std::string, int>AnimData;		//�A�j���[�V�����X�^�b�N
	//std::vector<std::vector<FBXModelData*>> Geometry;	//���b�V���f�[�^
	std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>>mMesh;	//�T�u���b�V�����܂ރ��b�V���f�[�^
	std::vector<FbxNode*>nodemeshes;	//���b�V���������m�[�h���X�g
	std::vector<FbxNode*>nodeAnimeMeshes;	//���b�V���������m�[�h���X�g(�A�j���[�V����)


	//�A�j���[�V�����p���\�[�X�̃L���b�V��
	FbxDataCache mCache;
};