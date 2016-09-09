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

		//�ǂݍ��݂ɐ��������ꍇ
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
						//�E�F�C�g�̌v�Z
						lWeight = (lWeight / lEndWeight) * 100;
						//�o�͒��_�f�[�^��������
						memcpy(lDstVertexArray, lSrcVertexArray, lVertexCount * sizeof(FbxVector4));
						for (int j = 0; j < lVertexCount; ++j) {
							FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lSrcVertexArray[j])*lWeight*0.01;
							lDstVertexArray[j] += lInfluence;
						}
					}
					else if (lStartShape&&lEndShape) {
						double lStartWeight = lFullWeights[lStartIndex];
						double lEndWeight = lFullWeights[lEndIndex];
						//�E�F�C�g�̌v�Z
						lWeight = (lWeight / lEndWeight) * 100;
						//�o�͒��_�f�[�^��������
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

			//2�̃X�L�j���O�̃E�F�C�g����������
			int lBlendWeightsCount = lSkinDeformer->GetControlPointIndicesCount();
			for (int lBWIndex = 0; lBWIndex < lBlendWeightsCount; ++lBWIndex) {
				double lBlendWeight = lSkinDeformer->GetControlPointBlendWeights()[lBWIndex];
				pVertexArray[lBWIndex] = lVertexArrayDQ[lBWIndex] * lBlendWeight + lVertexArrayLinear[lBWIndex] * (1 - lBlendWeight);
			}

			delete[]lVertexArrayLinear;
			delete[]lVertexArrayDQ;

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

					//�����ȃE�F�C�g�𖳎����Đ��K������
					if (lIndex >= lVertexCount)continue;

					double lWeight = lCluster->GetControlPointWeights()[k];

					if (lWeight == 0.0) {
						continue;
					}

					//				lInfluence = lVertexTransformMatrix;



					memcpy(&lInfluence, &lVertexTransformMatrix, sizeof(FbxAMatrix));

					FbxComputeDeformer::MatrixScale(lInfluence.Buffer()->Buffer(), lWeight);


					if (lClusterMode == FbxCluster::eAdditive) {
						//�Ńt�H�[�}�[���s��ɍ�������
						FbxComputeDeformer::MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
						lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];
						//�N���X�^�̃E�F�C�g��1.0�ɂ���
						lClusterWeight[lIndex] = 1.0;
					}
					else {
						FbxComputeDeformer::MatrixAdd(lClusterDeformation[lIndex].Buffer()->Buffer(), lInfluence);
						//���K�������W�I���g�����R���s���[�g�W�I���g���ɃE�F�C�g�����Z����
						lClusterWeight[lIndex] += lWeight;
					}
				}

			}

		}

		//���_�R�s�[
		for (int i = 0; i < lVertexCount; ++i)
		{
			//�R�s�[�����_
			FbxVector4 lSrcVertex = pVertexArray[i];
			//�E�F�C�g�̌v�Z���ʂ��܂ޒ��_
			FbxVector4& lDstVertex = pVertexArray[i];
			//�E�F�C�g�̎擾
			double lWeight = lClusterWeight[i];

			// Deform the vertex if there was at least a link with an influence on the vertex,
			//�E�F�C�g��0�łȂ��ꍇ�̂݌v�Z
			if (lWeight != 0.0)
			{
				//�x�N�g���ɍs��������������̂��擾����
				lDstVertex = lClusterDeformation[i].MultT(lSrcVertex);
				//���K������ꍇ�A�E�F�C�g�Ŋ���
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

		//�@���v�Z


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

		//�S�ẴN���X�^
		for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
		{
			//�f�t�H�[�}�[���擾
			FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
			//�N���X�^�����擾
			int lClusterCount = lSkinDeformer->GetClusterCount();
			for (int lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
			{
				//�N���X�^���擾
				FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
				//�N���X�^�Ƀ����N���Z�b�g����Ă��Ȃ���Ώ������Ȃ�
				if (!lCluster->GetLink())
					continue;

				//���_�̈ړ��s��
				FbxAMatrix lVertexTransformMatrix;
				ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);

				//�N�H�[�^�j�I���擾
				FbxQuaternion lQ = lVertexTransformMatrix.GetQ();
				FbxVector4 lT = lVertexTransformMatrix.GetT();
				FbxDualQuaternion lDualQuaternion(lQ, lT);
				int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
				for (int k = 0; k < lVertexIndexCount; ++k) {
					int lIndex = lCluster->GetControlPointIndices()[k];
					// Sometimes, the mesh can have less points than at the time of the skinning
					// because a smooth operator was active when skinning but has been deactivated during export.
					//���_�������傫�����l���擾�ł��Ă��܂����ꍇ�͖�����
					if (lIndex >= lVertexCount)
						continue;


					//�E�F�C�g�擾
					double lWeight = lCluster->GetControlPointWeights()[k];
					//�E�F�C�g��0�������ꍇ�͏����s�v
					if (lWeight == 0.0)
						continue;

					// Compute the influence of the link on the vertex.
					//�N�H�[�^�j�I���ƃE�F�C�g������
					FbxDualQuaternion lInfluence = lDualQuaternion * lWeight;
					if (lClusterMode == FbxCluster::eAdditive)
					{
						//�N�H�[�^�j�I�����Z�b�g
						lDQClusterDeformation[lIndex] = lInfluence;
						//�E�F�C�g��1.0�ɌŒ�
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
							//���_��ό`����l�����Z
							double lSign = lDQClusterDeformation[lIndex].GetFirstQuaternion().DotProduct(lDualQuaternion.GetFirstQuaternion());
							//�K�����������Z����悤�ɂ���
							if (lSign >= 0.0)
							{
								lDQClusterDeformation[lIndex] += lInfluence;
							}
							else
							{
								lDQClusterDeformation[lIndex] -= lInfluence;
							}
						}
						//�E�F�C�g�����Z
						lClusterWeight[lIndex] += lWeight;
					}

				}
			}
		}

		//���_������
		for (int i = 0; i < lVertexCount; ++i)
		{
			FbxVector4 lSrcVertex = pVertexArray[i];
			FbxVector4& lDstVertex = pVertexArray[i];
			double lWeightSum = lClusterWeight[i];

			// Deform the vertex if there was at least a link with an influence on the vertex,
			//�E�F�C�g��0�Ȃ疢����
			if (lWeightSum != 0.0)
			{
				lDQClusterDeformation[i].Normalize();
				lDstVertex = lDQClusterDeformation[i].Deform(lDstVertex);

				if (lClusterMode == FbxCluster::eNormalize)
				{
					// In the normalized link mode, a vertex is always totally influenced by the links. 
					//�Q�Ƃ������_�ɃE�F�C�g�Ŋ����Đ��K������
					lDstVertex /= lWeightSum;
				}
				else if (lClusterMode == FbxCluster::eTotalOne)
				{
					// In the total 1 link mode, a vertex can be partially influenced by the links. 
					//�e������Ƃ��낾������
					lSrcVertex *= (1.0 - lWeightSum);
					lDstVertex += lSrcVertex;
				}
			}
		}
		delete[] lDQClusterDeformation;
		delete[] lClusterWeight;

	}

	//���_��ϊ�����s����v�Z����
	void ComputeClusterDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix, FbxTime pTime, FbxPose* pPose)
	{
		FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();
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



		if (lClusterMode == FbxCluster::eAdditive&&pCluster->GetAssociateModel()) {
			pCluster->GetTransformAssociateModelMatrix(lAssociateGlobalInitPosition);

			//���f���̕ϊ�
			{
				GetGeometry(lAssociateGeometry, pCluster->GetAssociateModel());
				lAssociateGlobalInitPosition *= lAssociateGeometry;
				GetGlobalPosition(lAssociateGlobalCurrentPosition, pCluster->GetAssociateModel(), pTime, pPose);
			}

			//�ړ��s��̎擾
			{
				pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
				// Multiply lReferenceGlobalInitPosition by Geometric Transformation
				//�s��̍���
				GetGeometry(lReferenceGeometry, pMesh->GetNode());
				lReferenceGlobalInitPosition *= lReferenceGeometry;
			}
			//���݈ʒu���O���[�o���|�W�V�����Ƃ��Đݒ�
			//lReferenceGlobalCurrentPosition = pGlobalPosition;

			//�ړ��s��̎擾
			{
				pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
				// �s�񍇐�
				GetGeometry(lClusterGeometry, pCluster->GetLink());
				lClusterGlobalInitPosition *= lClusterGeometry;
				GetGlobalPosition(lClusterGlobalCurrentPosition, pCluster->GetLink(), pTime, pPose);
			}

			// Compute the shift of the link relative to the reference.
			//ModelM-1 * AssoM * AssoGX-1 * LinkGX * LinkM-1*ModelM
			//�ŏI�I�ȍs�񍇐�
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
					//���[���h���W�n�ɕϊ�����
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
