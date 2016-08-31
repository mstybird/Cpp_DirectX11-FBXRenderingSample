#pragma once
class FbxVBO {
public:
	struct SubMesh
	{
		SubMesh() : IndexOffset(0), TriangleCount(0) {}

		int IndexOffset;
		int TriangleCount;
	};
	FbxVBO();
	~FbxVBO();
	bool Initialize(const FbxMesh * pMesh);

	// Update vertex positions for deformed meshes.
	void UpdateVertexPosition(const FbxMesh * pMesh, const FbxVector4 * pVertices);
	int GetSubMeshCount() const { return mSubMeshes.GetCount(); }

	FbxArray<FbxFloat>lVertices;	//頂点配列
	FbxArray<FbxUInt>lIndices;	//インデクス
	FbxArray<FbxFloat>lNormals;	//法線配列
	FbxArray<FbxFloat>lUVs;		//UV配列
	FbxArray<SubMesh*> mSubMeshes;

private:
	bool mHasNormal;			//法線を持っているかどうか
	bool mHasUV;				//UV座標を持っているかどうか
	bool mAllByControlPoint; // Save data in VBO by control point or by polygon vertex.

};

