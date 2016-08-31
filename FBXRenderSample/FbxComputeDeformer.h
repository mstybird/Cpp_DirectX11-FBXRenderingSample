#pragma once
#include"DxFBXLoader.h"
namespace FbxComputeDeformer
{
	//���[���h���W���擾����
	void GetGlobalPosition(FbxAMatrix&pDstMatrix, FbxNode*pNode, const FbxTime&pTime, FbxPose*pPose, FbxAMatrix*pParentGlobalPosition = (fbxsdk::FbxAMatrix*)0);
	//�W�I���g���̃I�t�Z�b�g���擾����
	void GetGeometry(FbxAMatrix&pSrcMatrix, FbxNode*pNode);
	//�|�[�Y�擾
	FbxAMatrix GetPoseMatrix(FbxPose*pPose, int pNodeIndex);

	
	//�f�t�H�[�}�[���A�j���[�V���������֌W
	void ReadVertexCacheData(FbxMesh*pMesh, FbxTime&pTime, FbxVector4*pVertexArray);

	void ComputeShapeDeformation(FbxMesh*pMesh, FbxTime&pTime, FbxAnimLayer*pAnimLayer, FbxVector4*pVertexArray);

	void ComputeSkinDeformation(FbxAMatrix&pGlobalPosition, FbxMesh*pMesh, FbxTime&pTime, FbxVector4*pVertexArray, FbxPose*pPose);

	void ComputeLinearDeformation(FbxAMatrix&pGlobalPosition, FbxMesh*pMesh, FbxTime&pTime, FbxVector4*pVertexArray, FbxPose*pPose);

	void ComputeDualQuaternionDeformation(FbxAMatrix&pGlobalPosition, FbxMesh*pMesh, FbxTime&pTime, FbxVector4*pVertexArray, FbxPose*pPose);

	void ComputeClusterDeformation(FbxAMatrix&pGlobalPosition, FbxMesh*pMesh, FbxCluster*pCluster, FbxAMatrix& pVertexTransformMatrix, FbxTime pTime, FbxPose*pPose);

	void MatrixScale(FbxDouble*pMatrix, double pValue);
	void MatrixAddToDiagonal(FbxAMatrix&pMatrix, double pValue);
	void MatrixAdd(FbxDouble*pDstMatrix, FbxDouble*pSrcMatrix);
};

