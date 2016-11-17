#include "FbxComputeDeformer.h"
#include"fbxsdk.h"
namespace FbxComputeDeformer {
	//void ReadVertexCacheData(FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray);



	//void ComputeShapeDeformation(FbxMesh * pMesh, FbxTime & pTime, FbxAnimLayer * pAnimLayer, FbxVector4 * pVertexArray);


	//void ComputeLinearDeformation(FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose);


	//void ComputeDualQuaternionDeformation(FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose);
	//

	////’¸“_‚ð•ÏŠ·‚·‚és—ñ‚ðŒvŽZ‚·‚é
	//void ComputeClusterDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix, FbxTime pTime, FbxPose* pPose);


	void MatrixScale(FbxDouble* pMatrix, double pValue)
	{
		//FbxDouble* v = pMatrix.Buffer()->Buffer();

		for (int i = 0; i < 16; ++i) {
			pMatrix[i] *= pValue;
		}

	}

	void MatrixAddToDiagonal(FbxAMatrix & pMatrix, double pValue)
	{
		pMatrix[0][0] += pValue;
		pMatrix[1][1] += pValue;
		pMatrix[2][2] += pValue;
		pMatrix[3][3] += pValue;
	}

	void MatrixAdd(FbxDouble* pDstMatrix, FbxDouble*pSrcMatrix)
	{
		for (int i = 0; i < 16; ++i) {
			pDstMatrix[i] += pSrcMatrix[i];
		}
	}

}
