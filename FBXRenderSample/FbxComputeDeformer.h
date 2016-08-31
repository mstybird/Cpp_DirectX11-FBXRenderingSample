#pragma once
#include"DxFBXLoader.h"
namespace FbxComputeDeformer
{
	//ワールド座標を取得する
	void GetGlobalPosition(FbxAMatrix&pDstMatrix, FbxNode*pNode, const FbxTime&pTime, FbxPose*pPose, FbxAMatrix*pParentGlobalPosition = (fbxsdk::FbxAMatrix*)0);
	//ジオメトリのオフセットを取得する
	void GetGeometry(FbxAMatrix&pSrcMatrix, FbxNode*pNode);
	//ポーズ取得
	FbxAMatrix GetPoseMatrix(FbxPose*pPose, int pNodeIndex);

	
	//デフォーマー＆アニメーション処理関係
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

