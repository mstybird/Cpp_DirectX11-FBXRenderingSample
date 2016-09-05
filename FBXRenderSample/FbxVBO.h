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
	bool Initialize2(const FbxMesh * pMesh);

	// Update vertex positions for deformed meshes.
	void UpdateVertexPosition(const FbxMesh * pMesh, const FbxVector4 * pVertices);
	int GetSubMeshCount() const { return mSubMeshes.GetCount(); }

	FbxArray<FbxFloat>lVertices;	//���_�z��
	FbxArray<FbxUInt>lIndices;	//�C���f�N�X
	FbxArray<FbxFloat>lNormals;	//�@���z��
	FbxArray<FbxFloat>lUVs;		//UV�z��
	FbxArray<SubMesh*> mSubMeshes;

private:
	bool mHasNormal;			//�@���������Ă��邩�ǂ���
	bool mHasUV;				//UV���W�������Ă��邩�ǂ���
	bool mAllByControlPoint; // Save data in VBO by control point or by polygon vertex.

};

