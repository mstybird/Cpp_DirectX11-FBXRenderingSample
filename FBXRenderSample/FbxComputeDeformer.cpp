#include "FbxComputeDeformer.h"
#include"fbxsdk.h"
namespace FbxComputeDeformer {
	void ReadVertexCacheData(FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray)
	{
		FbxVertexCacheDeformer*lDeformer = static_cast<FbxVertexCacheDeformer*>(pMesh->GetDeformer(0, FbxDeformer::eVertexCache));
		FbxCache* lCache = lDeformer->GetCache();
		int lChannelIndex = lCache->GetChannelIndex(lDeformer->Channel.Get());
		unsigned int lVertexCount = (unsigned int)pMesh->GetControlPointsCount();
		bool lReadSucceed = false;
		float* lReadBuf = nullptr;
		unsigned int BufferSize = 0;

		if (lDeformer->Type.Get() != FbxVertexCacheDeformer::ePositions) {
			return;
		}

		unsigned int Length = 0;
		lCache->Read(NULL, Length, FBXSDK_TIME_ZERO, lChannelIndex);
		if (Length != lVertexCount * 3) {
			return;
		}
		lReadSucceed = lCache->Read(&lReadBuf, BufferSize, pTime, lChannelIndex);

		//読み込みに成功した場合
		if (lReadSucceed) {
			unsigned int lReadBufIndex = 0;

			while (lReadBufIndex < 3 * lVertexCount) {
				pVertexArray[lReadBufIndex / 3].mData[0] = lReadBuf[lReadBufIndex];
				++lReadBufIndex;
				pVertexArray[lReadBufIndex / 3].mData[1] = lReadBuf[lReadBufIndex];
				++lReadBufIndex;
				pVertexArray[lReadBufIndex / 3].mData[2] = lReadBuf[lReadBufIndex];
				++lReadBufIndex;
			}
		}


	}


	void ComputeShapeDeformation(FbxMesh * pMesh, FbxTime & pTime, FbxAnimLayer * pAnimLayer, FbxVector4 * pVertexArray)
	{
		int lVertexCount = pMesh->GetControlPointsCount();

		FbxVector4*lSrcVertexArray = pVertexArray;
		FbxVector4*lDstVertexArray = new FbxVector4[lVertexCount];
		memcpy(lDstVertexArray, pVertexArray, lVertexCount * sizeof(FbxVector4));

		int lBlendShapeDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);
		for (int lBlendShapeIndex = 0; lBlendShapeIndex < lBlendShapeDeformerCount; ++lBlendShapeIndex) {
			FbxBlendShape*lBlendShape = (FbxBlendShape*)pMesh->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);

			int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
			for (int lChannelIndex = 0; lChannelIndex < lBlendShapeChannelCount; ++lChannelIndex) {
				FbxBlendShapeChannel*lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
				if (lChannel) {
					FbxAnimCurve*lFCurve = pMesh->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer);
					
					if (!lFCurve)continue;
					double lWeight = lFCurve->Evaluate(pTime);

					int lShapeCount = lChannel->GetTargetShapeCount();
					double*lFullWeights = lChannel->GetTargetShapeFullWeights();

					int lStartIndex = -1;
					int lEndIndex = -1;
					for (int lShapeIndex = 0; lShapeIndex < lShapeCount; ++lShapeIndex) {
						if (lWeight > 0 && lWeight <= lFullWeights[0]) {
							lEndIndex = 0;
							break;
						}
						if (lWeight > lFullWeights[lShapeIndex] && lWeight < lFullWeights[lShapeIndex + 1]) {
							lStartIndex = lShapeIndex;
							lEndIndex = lShapeIndex + 1;
							break;
						}
					}

					FbxShape*lStartShape = nullptr;
					FbxShape*lEndShape = nullptr;

					if (lStartIndex > -1) {
						lStartShape = lChannel->GetTargetShape(lStartIndex);
					}
					if (lEndIndex > -1) {
						lEndShape = lChannel->GetTargetShape(lEndIndex);
					}
					if (lStartIndex == -1 && lEndShape) {
						double lEndWeight = lFullWeights[0];
						//ウェイトの計算
						lWeight = (lWeight / lEndWeight) * 100;
						//出力頂点データを初期化
						memcpy(lDstVertexArray, lSrcVertexArray, lVertexCount * sizeof(FbxVector4));
						for (int j = 0; j < lVertexCount; ++j) {
							FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lSrcVertexArray[j])*lWeight*0.01;
							lDstVertexArray[j] += lInfluence;
						}
					}
					else if (lStartShape&&lEndShape) {
						double lStartWeight = lFullWeights[lStartIndex];
						double lEndWeight = lFullWeights[lEndIndex];
						//ウェイトの計算
						lWeight = (lWeight / lEndWeight) * 100;
						//出力頂点データを初期化
						memcpy(lDstVertexArray, lStartShape->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

						for (int j = 0; j < lVertexCount; ++j) {
							FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lStartShape->GetControlPoints()[j])*lWeight*0.01;
							lDstVertexArray[j] += lInfluence;
						}
					}
				}
			}
		}
		memcpy(pVertexArray, lDstVertexArray, lVertexCount * sizeof(FbxVector4));
		delete[] lDstVertexArray;
	}

	void ComputeSkinDeformation(FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose)
	{
		FbxSkin*lSkinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);
		FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();

		if (lSkinningType == FbxSkin::eLinear || lSkinningType == FbxSkin::eRigid) {
			ComputeLinearDeformation(pGlobalPosition, pMesh, pTime, pVertexArray, pPose);
		}
		else if (lSkinningType == FbxSkin::eDualQuaternion) {
			ComputeDualQuaternionDeformation(pGlobalPosition, pMesh, pTime, pVertexArray, pPose);
		}
		else if (lSkinningType == FbxSkin::eBlend) {
			int lVertexCount = pMesh->GetControlPointsCount();
			FbxVector4*lVertexArrayLinear = new FbxVector4[lVertexCount];
			memcpy(lVertexArrayLinear, pMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
			FbxVector4*lVertexArrayDQ = new FbxVector4[lVertexCount];
			memcpy(lVertexArrayDQ, pMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

			ComputeLinearDeformation(pGlobalPosition, pMesh, pTime, lVertexArrayDQ, pPose);
			ComputeDualQuaternionDeformation(pGlobalPosition, pMesh, pTime, lVertexArrayDQ, pPose);

			//2つのスキニングのウェイトを合成する
			int lBlendWeightsCount = lSkinDeformer->GetControlPointIndicesCount();
			for (int lBWIndex = 0; lBWIndex < lBlendWeightsCount; ++lBWIndex) {
				double lBlendWeight = lSkinDeformer->GetControlPointBlendWeights()[lBWIndex];
				pVertexArray[lBWIndex] = lVertexArrayDQ[lBWIndex] * lBlendWeight + lVertexArrayLinear[lBWIndex] * (1 - lBlendWeight);
			}

		}
	}

	void ComputeLinearDeformation(FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose)
	{
		FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();
		int lVertexCount = pMesh->GetControlPointsCount();



		FbxAMatrix*lClusterDeformation = new FbxAMatrix[lVertexCount];

		memset(lClusterDeformation, 0, lVertexCount * sizeof(FbxAMatrix));
		double*lClusterWeight = new double[lVertexCount];
		memset(lClusterWeight, 0, lVertexCount * sizeof(double));
		if (lClusterMode == FbxCluster::eAdditive) {
			for (int i = 0; i < lVertexCount; ++i) {
				lClusterDeformation[i].SetIdentity();
			}
		}

		int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
		FbxAMatrix lInfluence;
		FbxAMatrix lVertexTransformMatrix;
		for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex) {

			FbxSkin*lSkinDeformer = (FbxSkin*)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
			int lClusterCount = lSkinDeformer->GetClusterCount();
			for (int lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex) {


				FbxCluster*lCluster = lSkinDeformer->GetCluster(lClusterIndex);
				if (!lCluster->GetLink()) {
					continue;
				}


				FbxComputeDeformer::ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);


				int lVertexIndexCount = lCluster->GetControlPointIndicesCount();


				for (int k = 0; k < lVertexIndexCount; ++k) {
					int lIndex = lCluster->GetControlPointIndices()[k];

					//小さなウェイトを無視して正規化する
					if (lIndex >= lVertexCount)continue;

					double lWeight = lCluster->GetControlPointWeights()[k];

					if (lWeight == 0.0) {
						continue;
					}

					//				lInfluence = lVertexTransformMatrix;



					memcpy(&lInfluence, &lVertexTransformMatrix, sizeof(FbxAMatrix));

					FbxComputeDeformer::MatrixScale(lInfluence.Buffer()->Buffer(), lWeight);


					if (lClusterMode == FbxCluster::eAdditive) {
						//でフォーマーを行列に合成する
						FbxComputeDeformer::MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
						lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];
						//クラスタのウェイトを1.0にする
						lClusterWeight[lIndex] = 1.0;
					}
					else {
						FbxComputeDeformer::MatrixAdd(lClusterDeformation[lIndex].Buffer()->Buffer(), lInfluence);
						//正規化したジオメトリかコンピュートジオメトリにウェイトを加算する
						lClusterWeight[lIndex] += lWeight;
					}
				}

			}

		}

		//頂点コピー
		for (int i = 0; i < lVertexCount; ++i)
		{
			//コピー元頂点
			FbxVector4 lSrcVertex = pVertexArray[i];
			//ウェイトの計算結果を含む頂点
			FbxVector4& lDstVertex = pVertexArray[i];
			//ウェイトの取得
			double lWeight = lClusterWeight[i];

			// Deform the vertex if there was at least a link with an influence on the vertex,
			//ウェイトが0でない場合のみ計算
			if (lWeight != 0.0)
			{
				//ベクトルに行列を合成したものを取得する
				lDstVertex = lClusterDeformation[i].MultT(lSrcVertex);
				//正規化する場合、ウェイトで割る
				if (lClusterMode == FbxCluster::eNormalize)
				{
					// In the normalized link mode, a vertex is always totally influenced by the links. 
					lDstVertex /= lWeight;
				}
				//
				else if (lClusterMode == FbxCluster::eTotalOne)
				{
					// In the total 1 link mode, a vertex can be partially influenced by the links. 
					lSrcVertex *= (1.0 - lWeight);
					lDstVertex += lSrcVertex;
				}
			}
		}

		//法線計算


		delete[] lClusterDeformation;
		delete[] lClusterWeight;

	}

	void ComputeDualQuaternionDeformation(FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose)
	{
		FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

		int lVertexCount = pMesh->GetControlPointsCount();
		int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

		FbxDualQuaternion*lDQClusterDeformation = new FbxDualQuaternion[lVertexCount];
		memset(lDQClusterDeformation, 0, lVertexCount * sizeof(FbxDualQuaternion));
		double*lClusterWeight = new double[lVertexCount];
		memset(lClusterWeight, 0, lVertexCount * sizeof(double));

		//全てのクラスタ
		for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
		{
			//デフォーマーを取得
			FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
			//クラスタ数を取得
			int lClusterCount = lSkinDeformer->GetClusterCount();
			for (int lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
			{
				//クラスタを取得
				FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
				//クラスタにリンクがセットされていなければ処理しない
				if (!lCluster->GetLink())
					continue;

				//頂点の移動行列
				FbxAMatrix lVertexTransformMatrix;
				ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);

				//クォータニオン取得
				FbxQuaternion lQ = lVertexTransformMatrix.GetQ();
				FbxVector4 lT = lVertexTransformMatrix.GetT();
				FbxDualQuaternion lDualQuaternion(lQ, lT);
				int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
				for (int k = 0; k < lVertexIndexCount; ++k) {
					int lIndex = lCluster->GetControlPointIndices()[k];
					// Sometimes, the mesh can have less points than at the time of the skinning
					// because a smooth operator was active when skinning but has been deactivated during export.
					//頂点数よりも大きい数値が取得できてしまった場合は未処理
					if (lIndex >= lVertexCount)
						continue;


					//ウェイト取得
					double lWeight = lCluster->GetControlPointWeights()[k];
					//ウェイトが0だった場合は処理不要
					if (lWeight == 0.0)
						continue;

					// Compute the influence of the link on the vertex.
					//クォータニオンとウェイトを合成
					FbxDualQuaternion lInfluence = lDualQuaternion * lWeight;
					if (lClusterMode == FbxCluster::eAdditive)
					{
						//クォータニオンをセット
						lDQClusterDeformation[lIndex] = lInfluence;
						//ウェイトを1.0に固定
						lClusterWeight[lIndex] = 1.0;
					}
					else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
					{
						if (lClusterIndex == 0)
						{
							lDQClusterDeformation[lIndex] = lInfluence;
						}
						else
						{
							// Add to the sum of the deformations on the vertex.
							// Make sure the deformation is accumulated in the same rotation direction. 
							// Use dot product to judge the sign.
							//頂点を変形する値を加算
							double lSign = lDQClusterDeformation[lIndex].GetFirstQuaternion().DotProduct(lDualQuaternion.GetFirstQuaternion());
							//必ず正数を加算するようにする
							if (lSign >= 0.0)
							{
								lDQClusterDeformation[lIndex] += lInfluence;
							}
							else
							{
								lDQClusterDeformation[lIndex] -= lInfluence;
							}
						}
						//ウェイトを加算
						lClusterWeight[lIndex] += lWeight;
					}

				}
			}
		}

		//頂点数分回す
		for (int i = 0; i < lVertexCount; ++i)
		{
			FbxVector4 lSrcVertex = pVertexArray[i];
			FbxVector4& lDstVertex = pVertexArray[i];
			double lWeightSum = lClusterWeight[i];

			// Deform the vertex if there was at least a link with an influence on the vertex,
			//ウェイトが0なら未処理
			if (lWeightSum != 0.0)
			{
				lDQClusterDeformation[i].Normalize();
				lDstVertex = lDQClusterDeformation[i].Deform(lDstVertex);

				if (lClusterMode == FbxCluster::eNormalize)
				{
					// In the normalized link mode, a vertex is always totally influenced by the links. 
					//参照した頂点にウェイトで割って正規化する
					lDstVertex /= lWeightSum;
				}
				else if (lClusterMode == FbxCluster::eTotalOne)
				{
					// In the total 1 link mode, a vertex can be partially influenced by the links. 
					//影響するところだけ合成
					lSrcVertex *= (1.0 - lWeightSum);
					lDstVertex += lSrcVertex;
				}
			}
		}
		delete[] lDQClusterDeformation;
		delete[] lClusterWeight;

	}

	//頂点を変換する行列を計算する
	void ComputeClusterDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix, FbxTime pTime, FbxPose* pPose)
	{
		FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();
		FbxAMatrix lReferenceGlobalInitPosition;	//初期位置
		FbxAMatrix lReferenceGlobalCurrentPosition;	//現在位置
		FbxAMatrix lAssociateGlobalInitPosition;	//初期位置
		FbxAMatrix lAssociateGlobalCurrentPosition;	//現在位置
		FbxAMatrix lClusterGlobalInitPosition;		//初期位置
		FbxAMatrix lClusterGlobalCurrentPosition;	//現在位置

		FbxAMatrix lReferenceGeometry;
		FbxAMatrix lAssociateGeometry;
		FbxAMatrix lClusterGeometry;

		FbxAMatrix lClusterRelativeInitPosition;
		FbxAMatrix lClusterRelativeCurrentPositionInverse;



		if (lClusterMode == FbxCluster::eAdditive&&pCluster->GetAssociateModel()) {
			pCluster->GetTransformAssociateModelMatrix(lAssociateGlobalInitPosition);

			//モデルの変換
			{
				GetGeometry(lAssociateGeometry, pCluster->GetAssociateModel());
				lAssociateGlobalInitPosition *= lAssociateGeometry;
				GetGlobalPosition(lAssociateGlobalCurrentPosition, pCluster->GetAssociateModel(), pTime, pPose);
			}

			//移動行列の取得
			{
				pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
				// Multiply lReferenceGlobalInitPosition by Geometric Transformation
				//行列の合成
				GetGeometry(lReferenceGeometry, pMesh->GetNode());
				lReferenceGlobalInitPosition *= lReferenceGeometry;
			}
			//現在位置をグローバルポジションとして設定
			//lReferenceGlobalCurrentPosition = pGlobalPosition;

			//移動行列の取得
			{
				pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
				// 行列合成
				GetGeometry(lClusterGeometry, pCluster->GetLink());
				lClusterGlobalInitPosition *= lClusterGeometry;
				GetGlobalPosition(lClusterGlobalCurrentPosition, pCluster->GetLink(), pTime, pPose);
			}

			// Compute the shift of the link relative to the reference.
			//ModelM-1 * AssoM * AssoGX-1 * LinkGX * LinkM-1*ModelM
			//最終的な行列合成
			{
				pVertexTransformMatrix = lReferenceGlobalInitPosition.Inverse() * lAssociateGlobalInitPosition * lAssociateGlobalCurrentPosition.Inverse() *
					lClusterGlobalCurrentPosition * lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

			}


		}
		else {

			pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
			lReferenceGlobalCurrentPosition = pGlobalPosition;
			// Multiply lReferenceGlobalInitPosition by Geometric Transformation
			GetGeometry(lReferenceGeometry, pMesh->GetNode());
			lReferenceGlobalInitPosition *= lReferenceGeometry;

			// Get the link initial global position and the link current global position.
			pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
			GetGlobalPosition(lClusterGlobalCurrentPosition, pCluster->GetLink(), pTime, pPose);
			// Compute the initial position of the link relative to the reference.
			lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

			// Compute the current position of the link relative to the reference.
			lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

			// Compute the shift of the link relative to the reference.
			pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;
		}


	}

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

	void GetGlobalPosition(FbxAMatrix&pDstMatrix, FbxNode * pNode, const FbxTime & pTime, FbxPose * pPose, FbxAMatrix * pParentGlobalPosition)
	{
		bool lPositionFound = false;

		if (pPose) {
			int lNodeIndex = pPose->Find(pNode);

			if (lNodeIndex > -1) {
				if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex)) {
					pDstMatrix = FbxComputeDeformer::GetPoseMatrix(pPose, lNodeIndex);
				}
				else {
					//ワールド座標系に変換する
					FbxAMatrix lParentGlobalPosition;
					if (pParentGlobalPosition) {
						//lParentGlobalPosition = *pParentGlobalPosition;
						memcpy(&lParentGlobalPosition, pParentGlobalPosition, sizeof(FbxAMatrix));
					}
					else {
						if (pNode->GetParent()) {
							FbxComputeDeformer::GetGlobalPosition(lParentGlobalPosition, pNode->GetParent(), pTime, pPose);
						}
					}

					FbxAMatrix lLocalPosition = FbxComputeDeformer::GetPoseMatrix(pPose, lNodeIndex);
					pDstMatrix = lParentGlobalPosition*lLocalPosition;

				}
				lPositionFound = true;
			}

		}

		if (!lPositionFound) {
			pDstMatrix = pNode->EvaluateGlobalTransform(pTime);
		}
	}

	FbxAMatrix GetPoseMatrix(FbxPose * pPose, int pNodeIndex)
	{
		FbxAMatrix lPoseMatrix;
		FbxMatrix lMatrix;

		lPoseMatrix;
		lMatrix = pPose->GetMatrix(pNodeIndex);
		memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

		return lPoseMatrix;
	}

	void GetGeometry(FbxAMatrix&pSrcMatrix, FbxNode * pNode)
	{
		//const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		//const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		//const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

		pSrcMatrix.SetTRS(
			pNode->GetGeometricTranslation(FbxNode::eSourcePivot),
			pNode->GetGeometricRotation(FbxNode::eSourcePivot),
			pNode->GetGeometricScaling(FbxNode::eSourcePivot)

		);

		//	return FbxAMatrix(lT, lR, lS);
	}


}
