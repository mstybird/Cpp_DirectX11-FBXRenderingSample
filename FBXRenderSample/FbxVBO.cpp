#include"DX11FbxLoader.h"

namespace {
	const float ANGLE_TO_RADIAN = 3.1415926f / 180.f;
	const FbxFloat BLACK_COLOR[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const FbxFloat GREEN_COLOR[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	const FbxFloat WHITE_COLOR[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const FbxFloat WIREFRAME_COLOR[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	const int TRIANGLE_VERTEX_COUNT = 3;
	//���_��̗v�f��
	const int VERTEX_STRIDE = 4;
	//�@����̗v�f��
	const int NORMAL_STRIDE = 3;
	//UV��̗v�f��
	const int UV_STRIDE = 2;

	FbxDouble3 GetMaterialProperty(const FbxSurfaceMaterial*pMaterial, const char*pPropertyName, const char*pFactorPropertyName, FbxUInt&pTextureName) {
		FbxDouble3 lResult(0, 0, 0);
		const FbxProperty lProperty = pMaterial->FindProperty(pPropertyName);
		const FbxProperty lFactorProperty = pMaterial->FindProperty(pFactorPropertyName);
		if (lProperty.IsValid() && lFactorProperty.IsValid()) {
			lResult = lProperty.Get<FbxDouble3>();
			double lFactor = lFactorProperty.IsValid();
			if (lFactor != 1) {
				lResult[0] *= lFactor;
				lResult[1] *= lFactor;
				lResult[2] *= lFactor;
			}
		}

		if (lProperty.IsValid()) {
			const int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
			if (lTextureCount) {
				const FbxFileTexture*lTexture = lProperty.GetSrcObject<FbxFileTexture>();
				if (lTexture&&lTexture->GetUserDataPtr()) {
					pTextureName = *(static_cast<FbxUInt*>(lTexture->GetUserDataPtr()));
					const char* dummy = (char*)lTexture->GetUserDataPtr();
				}
			}
		}

		return lResult;
	}

}


FbxVBO::FbxVBO()
{

}

FbxVBO::~FbxVBO()
{
	auto subMeshes = mSubMeshes.GetArray();
	for (int i = 0; i < mSubMeshes.Size(); i++) {
		delete subMeshes[i];
	}
}

bool FbxVBO::Initialize(const FbxMesh * pMesh)
{
	//�w�肵�����b�V���Ƀm�[�h���Ȃ���Ώ������Ȃ�
	if (!pMesh->GetNode())return false;
	//�|���S��������sp��
	const int lPolygonCount = pMesh->GetPolygonCount();


	FbxLayerElementArrayTemplate<int>*lMaterialIndice = nullptr;
	FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;

	//�}�e���A�������݂��邩�ǂ���
	if (pMesh->GetElementMaterial()) {
		lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
		lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
		if (lMaterialIndice&&lMaterialMappingMode == FbxGeometryElement::eByPolygon) {
			FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);

			if (lMaterialIndice->GetCount() == lPolygonCount) {
				//�t�F�C�X�}�e���A���������[�v
				for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; lPolygonIndex++) {
					//�|���S���̃}�e���A���C���f�b�N�X���擾
					const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
					if (mSubMeshes.GetCount() < lMaterialIndex + 1) {
						mSubMeshes.Resize(lMaterialIndex + 1);
					}
					//�擾�����C���f�N�X�Ƀ��b�V�����o�^����Ă��Ȃ���Γo�^
					if (mSubMeshes[lMaterialIndex] == nullptr) {
						mSubMeshes[lMaterialIndex] = new SubMesh;
					}
					//�O�p�`�̌����J�E���g
					mSubMeshes[lMaterialIndex]->TriangleCount += 1;
				}

				//���o�^���b�V���ɑ΂���nullptr���������ꍇ�A�o�^����
				for (int i = 0; i < mSubMeshes.GetCount(); i++) {
					if (mSubMeshes[i] == NULL) {
						mSubMeshes[i] = new SubMesh;
					}
				}

				const int lMaterialCount = mSubMeshes.GetCount();
				int lOffset = 0;
				for (int lIndex = 0; lIndex < lMaterialCount; lIndex++) {
					mSubMeshes[lIndex]->IndexOffset = lOffset;
					//�O�p�`�̌����C���f�b�N�X�I�t�Z�b�g�����炷
					lOffset += mSubMeshes[lIndex]->TriangleCount * 3;
					mSubMeshes[lIndex]->TriangleCount = 0;
				}
				FBX_ASSERT(lOffset == lPolygonCount * 3);

			}

		}
	}

	//�t�F�C�X�|���S��������Ȃ��������A�T�u���b�V���͂P�쐬���Ă���
	if (mSubMeshes.GetCount() == 0)
	{
		mSubMeshes.Resize(1);
		mSubMeshes[0] = new SubMesh();
	}

	//�@�������݂��邩
	mHasNormal = pMesh->GetElementNormalCount() > 0;
	//UV���W�����݂��邩
	mHasUV = pMesh->GetElementUVCount() > 0;

	FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
	FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;

	//�@�����������ꍇ
	if (mHasNormal) {
		lNormalMappingMode = pMesh->GetElementNormal(0)->GetMappingMode();
		if (lNormalMappingMode == FbxGeometryElement::eNone) {
			mHasNormal = false;
		}
		if (mHasNormal&&lNormalMappingMode != FbxGeometryElement::eByControlPoint) {
			mAllByControlPoint = false;
		}
	}

	//UV���W���������ꍇ
	if (mHasUV) {
		lUVMappingMode = pMesh->GetElementUV(0)->GetMappingMode();
		if (lUVMappingMode == FbxGeometryElement::eNone) {
			mHasUV = false;
		}
		if (mHasUV&&lUVMappingMode != FbxGeometryElement::eByControlPoint) {
			mAllByControlPoint = false;
		}
	}

	//���_�����J�E���g
	int lPolygonVertexCount = pMesh->GetControlPointsCount();

	//�@���AUV���W�ǂ��炩���R���g���[���|�C���g�Ōq���Ȃ����@�������ꍇ�A�|���S����*3(�|���S����S�ĎO�p�`�ɐ��K�����Ă��邱�Ƃ��O��)�����ۂ̒��_���Ƃ���
	if (!mAllByControlPoint) {
		lPolygonVertexCount = lPolygonCount*TRIANGLE_VERTEX_COUNT;
	}



	lVertices.Reserve(lPolygonVertexCount*VERTEX_STRIDE);
	lIndices.Reserve(lPolygonCount*TRIANGLE_VERTEX_COUNT);
	if (mHasNormal) {
		lNormals.Reserve(lPolygonVertexCount*NORMAL_STRIDE);
	}

	FbxStringList lUVNames;
	pMesh->GetUVSetNames(lUVNames);
	const char* lUVName = nullptr;
	if (mHasUV&&lUVNames.GetCount()) {
		lUVs.Reserve(lPolygonVertexCount*UV_STRIDE);
		lUVName = lUVNames[0];
	}





	//�R���g���[���|�C���g�𗊂�ɒ��_���Ȃ�

	//�R���g���[���|�C���g�擾
	const FbxVector4 * lControlPoints = pMesh->GetControlPoints();
	FbxVector4 lCurrentVertex;
	FbxVector4 lCurrentNormal;
	FbxVector2 lCurrentUV;

	if (mAllByControlPoint) {
		const FbxGeometryElementNormal*lNormalElement = nullptr;
		const FbxGeometryElementUV*lUVElement = nullptr;
		if (mHasNormal) {
			//�@���擾
			lNormalElement = pMesh->GetElementNormal(0);
		}
		if (mHasUV) {
			//UV���W�擾
			lUVElement = pMesh->GetElementUV(0);
		}
		for (int lIndex = 0; lIndex < lPolygonVertexCount; lIndex++) {

			//���_���擾
			lCurrentVertex = lControlPoints[lIndex];

			lVertices.SetAt(lIndex * VERTEX_STRIDE + 0, static_cast<float>(lCurrentVertex[0]));
			lVertices.SetAt(lIndex * VERTEX_STRIDE + 1, static_cast<float>(lCurrentVertex[1]));
			lVertices.SetAt(lIndex * VERTEX_STRIDE + 2, static_cast<float>(lCurrentVertex[2]));
			lVertices.SetAt(lIndex * VERTEX_STRIDE + 3, 1.0f);

			//lVertices.push_back(s);
			//lVertices.push_back(static_cast<float>(lCurrentVertex[1]));
			//lVertices.push_back(static_cast<float>(lCurrentVertex[2]));
			//lVertices.push_back(1.0f);
			//lVertices[lIndex * VERTEX_STRIDE] = ;
			//lVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
			//lVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
			//lVertices[lIndex * VERTEX_STRIDE + 3] = 1;//w��1�Œ�


			//�@��������Ζ@���擾
			if (mHasNormal) {
				int lNormalIndex = lIndex;




				//�@�����C���f�b�N�X������΂�����g���Ė@�����擾����
				if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
				}
				lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
				lNormals.SetAt(lIndex*NORMAL_STRIDE + 0, static_cast<float>(lCurrentNormal[0]));
				lNormals.SetAt(lIndex*NORMAL_STRIDE + 1, static_cast<float>(lCurrentNormal[1]));
				lNormals.SetAt(lIndex*NORMAL_STRIDE + 2, static_cast<float>(lCurrentNormal[2]));
			}


			//UV���W�������UV�擾
			if (mHasUV)
			{
				int lUVIndex = lIndex;
				if (lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				{
					lUVIndex = lUVElement->GetIndexArray().GetAt(lIndex);
				}
				lCurrentUV = lUVElement->GetDirectArray().GetAt(lUVIndex);

				lUVs.SetAt(lIndex*UV_STRIDE + 0, static_cast<float>(lCurrentUV[0]));
				lUVs.SetAt(lIndex*UV_STRIDE + 1, static_cast<float>(lCurrentUV[1]));

				//lUVs.push_back(static_cast<float>(lCurrentUV[0]));
				//lUVs.push_back(static_cast<float>(lCurrentUV[1]));
			}

		}

	}

	int lVertexCount = 0;
	for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; lPolygonIndex++) {
		//�}�e���A���t�F�C�X���擾
		int lMaterialIndex = 0;
		if (lMaterialIndice&&lMaterialMappingMode == FbxGeometryElement::eByPolygon) {
			lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
		}

		const int lIndexOffset = mSubMeshes[lMaterialIndex]->IndexOffset + mSubMeshes[lMaterialIndex]->TriangleCount * 3;

		/*
		��������

		*/
		//�O�p�`�|���S���̒��_�������[�v
		for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; lVerticeIndex++) {
			const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

			if (mAllByControlPoint) {

				lIndices.SetAt(lIndexOffset + lVerticeIndex, static_cast<FbxUInt>(lControlPointIndex));
			}
			else {
				lIndices.SetAt(lIndexOffset + lVerticeIndex, static_cast<FbxUInt>(lVertexCount));
				lCurrentVertex = lControlPoints[lControlPointIndex];
				lVertices.SetAt(lVertexCount * VERTEX_STRIDE + 0, static_cast<float>(lCurrentVertex[0]));
				lVertices.SetAt(lVertexCount * VERTEX_STRIDE + 1, static_cast<float>(lCurrentVertex[1]));
				lVertices.SetAt(lVertexCount * VERTEX_STRIDE + 2, static_cast<float>(lCurrentVertex[2]));
				lVertices.SetAt(lVertexCount * VERTEX_STRIDE + 3, 1);

				if (mHasNormal)
				{
					pMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);
					lNormals.SetAt(lVertexCount * NORMAL_STRIDE + 0, static_cast<float>(lCurrentNormal[0]));
					lNormals.SetAt(lVertexCount * NORMAL_STRIDE + 1, static_cast<float>(lCurrentNormal[1]));
					lNormals.SetAt(lVertexCount * NORMAL_STRIDE + 2, static_cast<float>(lCurrentNormal[2]));
				}

				if (mHasUV)
				{
					bool lUnmappedUV;
					pMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, lCurrentUV, lUnmappedUV);
					lUVs.SetAt(lVertexCount * UV_STRIDE + 0, static_cast<float>(lCurrentUV[0]));
					lUVs.SetAt(lVertexCount * UV_STRIDE + 1, static_cast<float>(lCurrentUV[1]));
				}
			}
			lVertexCount++;
		}
		mSubMeshes[lMaterialIndex]->TriangleCount += 1;
	}
	return true;
}

void FbxVBO::UpdateVertexPosition(const FbxMesh * pMesh, const FbxVector4 * pVertices)
{
	lVertices.Clear();
	int lVertexCount = 0;

	if (mAllByControlPoint) {
		lVertexCount = pMesh->GetControlPointsCount();
		lVertices.Reserve(lVertexCount*VERTEX_STRIDE);
		for (int lIndex = 0; lIndex < lVertexCount; lIndex++) {
			lVertices.SetAt(lIndex*VERTEX_STRIDE + 0, static_cast<float>(pVertices[lIndex][0]));
			lVertices.SetAt(lIndex*VERTEX_STRIDE + 1, static_cast<float>(pVertices[lIndex][1]));
			lVertices.SetAt(lIndex*VERTEX_STRIDE + 2, static_cast<float>(pVertices[lIndex][2]));
			lVertices.SetAt(lIndex*VERTEX_STRIDE + 3, 1.0f);
		}
	}
	else {
		const int lPolygonCount = pMesh->GetPolygonCount();
		lVertexCount = lPolygonCount*TRIANGLE_VERTEX_COUNT;
		lVertices.Reserve(lVertexCount*VERTEX_STRIDE);

		lVertexCount = 0;
		for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; lPolygonIndex++) {
			for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; lVerticeIndex++) {
				//�w�肵���|���S���̎w�肵�����_���擾
				const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

				lVertices.SetAt(lVertexCount*VERTEX_STRIDE + 0, static_cast<float>(pVertices[lControlPointIndex][0]));
				lVertices.SetAt(lVertexCount*VERTEX_STRIDE + 1, static_cast<float>(pVertices[lControlPointIndex][1]));
				lVertices.SetAt(lVertexCount*VERTEX_STRIDE + 2, static_cast<float>(pVertices[lControlPointIndex][2]));
				lVertices.SetAt(lVertexCount*VERTEX_STRIDE + 3, 1.0f);
				lVertexCount++;
			}
		}

	}

}
