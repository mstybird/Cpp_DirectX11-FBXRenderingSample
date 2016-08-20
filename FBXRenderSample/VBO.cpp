#include"MAIN.h"

namespace {
	const float ANGLE_TO_RADIAN = 3.1415926f / 180.f;
	const FbxFloat BLACK_COLOR[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const FbxFloat GREEN_COLOR[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	const FbxFloat WHITE_COLOR[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const FbxFloat WIREFRAME_COLOR[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	const int TRIANGLE_VERTEX_COUNT = 3;
	//頂点一つの要素数
	const int VERTEX_STRIDE = 4;
	//法線一つの要素数
	const int NORMAL_STRIDE = 3;
	//UV一つの要素数
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


MYVBO::MYVBO()
{
	//VBOの初期化
	for (int lVBOIndex = 0; lVBOIndex < VBO_COUNT; lVBOIndex++) {
		mVBONames[lVBOIndex] = 0;
	}
}

MYVBO::~MYVBO()
{

}

bool MYVBO::Initialize(const FbxMesh * pMesh)
{
	//指定したメッシュにノードがなければ処理しない
	if (!pMesh->GetNode())return false;
	//ポリゴン数を首都p区
	const int lPolygonCount = pMesh->GetPolygonCount();


	FbxLayerElementArrayTemplate<int>*lMaterialIndice = nullptr;
	FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;

	//マテリアルが存在するかどうか
	if (pMesh->GetElementMaterial()) {
		lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
		lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
		if (lMaterialIndice&&lMaterialMappingMode == FbxGeometryElement::eByPolygon) {
			FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);

			if (lMaterialIndice->GetCount() == lPolygonCount) {
				//フェイスマテリアル数分ループ
				for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; lPolygonIndex++) {
					//ポリゴンのマテリアルインデックスを取得
					const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
					if (mSubMeshes.GetCount() < lMaterialIndex + 1) {
						mSubMeshes.Resize(lMaterialIndex + 1);
					}
					//取得したインデクスにメッシュが登録されていなければ登録
					if (mSubMeshes[lMaterialIndex] == nullptr) {
						mSubMeshes[lMaterialIndex] = new SubMesh;
					}
					//三角形の個数をカウント
					mSubMeshes[lMaterialIndex]->TriangleCount += 1;
				}

				//未登録メッシュに対してnullptrがあった場合、登録する
				for (int i = 0; i < mSubMeshes.GetCount(); i++) {
					if (mSubMeshes[i] == NULL) {
						mSubMeshes[i] = new SubMesh;
					}
				}

				const int lMaterialCount = mSubMeshes.GetCount();
				int lOffset = 0;
				for (int lIndex = 0; lIndex < lMaterialCount; lIndex++) {
					mSubMeshes[lIndex]->IndexOffset = lOffset;
					//三角形の個数分インデックスオフセットをずらす
					lOffset += mSubMeshes[lIndex]->TriangleCount * 3;
					mSubMeshes[lIndex]->TriangleCount = 0;
				}
				FBX_ASSERT(lOffset == lPolygonCount * 3);

			}

		}
	}

	//フェイスポリゴンが一つもなかった時、サブメッシュは１つ作成しておく
	if (mSubMeshes.GetCount() == 0)
	{
		mSubMeshes.Resize(1);
		mSubMeshes[0] = new SubMesh();
	}

	//法線が存在するか
	mHasNormal = pMesh->GetElementNormalCount() > 0;
	//UV座標が存在するか
	mHasUV = pMesh->GetElementUVCount() > 0;

	FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
	FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;

	//法線があった場合
	if (mHasNormal) {
		lNormalMappingMode = pMesh->GetElementNormal(0)->GetMappingMode();
		if (lNormalMappingMode == FbxGeometryElement::eNone) {
			mHasNormal = false;
		}
		if (mHasNormal&&lNormalMappingMode != FbxGeometryElement::eByControlPoint) {
			mAllByControlPoint = false;
		}
	}

	//UV座標があった場合
	if (mHasUV) {
		lUVMappingMode = pMesh->GetElementUV(0)->GetMappingMode();
		if (lUVMappingMode == FbxGeometryElement::eNone) {
			mHasUV = false;
		}
		if (mHasUV&&lUVMappingMode != FbxGeometryElement::eByControlPoint) {
			mAllByControlPoint = false;
		}
	}

	//頂点数をカウント
	int lPolygonVertexCount = pMesh->GetControlPointsCount();

	//法線、UV座標どちらかがコントロールポイントで繋がない方法だった場合、ポリゴン数*3(ポリゴンを全て三角形に正規化していることが前提)を実際の頂点数とする
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





	//コントロールポイントを頼りに頂点をつなぐ

	//コントロールポイント取得
	const FbxVector4 * lControlPoints = pMesh->GetControlPoints();
	FbxVector4 lCurrentVertex;
	FbxVector4 lCurrentNormal;
	FbxVector2 lCurrentUV;

	if (mAllByControlPoint) {
		const FbxGeometryElementNormal*lNormalElement = nullptr;
		const FbxGeometryElementUV*lUVElement = nullptr;
		if (mHasNormal) {
			//法線取得
			lNormalElement = pMesh->GetElementNormal(0);
		}
		if (mHasUV) {
			//UV座標取得
			lUVElement = pMesh->GetElementUV(0);
		}
		for (int lIndex = 0; lIndex < lPolygonVertexCount; lIndex++) {

			//頂点を取得
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
			//lVertices[lIndex * VERTEX_STRIDE + 3] = 1;//wは1固定


			//法線があれば法線取得
			if (mHasNormal) {
				int lNormalIndex = lIndex;
				//法線もインデックスがあればそれを使って法線を取得する
				if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
				}
				lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
				lNormals.SetAt(lIndex*NORMAL_STRIDE + 0, static_cast<float>(lCurrentNormal[0]));
				lNormals.SetAt(lIndex*NORMAL_STRIDE + 1, static_cast<float>(lCurrentNormal[1]));
				lNormals.SetAt(lIndex*NORMAL_STRIDE + 2, static_cast<float>(lCurrentNormal[2]));
			}


			//UV座標があればUV取得
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
		//マテリアルフェイスを取得
		int lMaterialIndex = 0;
		if (lMaterialIndice&&lMaterialMappingMode == FbxGeometryElement::eByPolygon) {
			lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
		}

		const int lIndexOffset = mSubMeshes[lMaterialIndex]->IndexOffset + mSubMeshes[lMaterialIndex]->TriangleCount * 3;

		/*
		ここから

		*/
		//三角形ポリゴンの頂点数分ループ
		for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; lVerticeIndex++) {
			const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

			if (mAllByControlPoint) {

				lIndices.SetAt(lIndexOffset + lVerticeIndex,static_cast<FbxUInt>(lControlPointIndex));
			}
			else {
				lIndices.SetAt(lIndexOffset + lVerticeIndex, static_cast<FbxUInt>(lVertexCount));
				lCurrentVertex = lControlPoints[lControlPointIndex];
				lVertices.SetAt(lVertexCount * VERTEX_STRIDE + 0, static_cast<float>(lCurrentVertex[0]));
				lVertices.SetAt(lVertexCount * VERTEX_STRIDE + 1, static_cast<float>(lCurrentVertex[1]));
				lVertices.SetAt(lVertexCount * VERTEX_STRIDE + 2, static_cast<float>(lCurrentVertex[2]));
				lVertices.SetAt(lVertexCount * VERTEX_STRIDE + 3,1);

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

void MYVBO::UpdateVertexPosition(const FbxMesh * pMesh, const FbxVector4 * pVertices)
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
				//指定したポリゴンの指定した頂点を取得
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
