#pragma once
//#include<d3dx9.h>
#include<vector>
#include<memory>
#include<array>
#include"DXMath.hpp"
class DXTexture;
class DX11TextureManager;

struct ColorChannel {
	ColorChannel();
	~ColorChannel();
	void CreateTexture();
	std::string TextureName;	//�e�N�X�`���t�@�C���p�X
//	DXTexture*mTexture;			//�e�N�X�`�����\�[�X
	DXTexture*mTexture;
	int mTextureID;
	float Color[4];				//�}�e���A���J���[


	static std::unique_ptr<DX11TextureManager>sTextureManager;
	static int sTextureCounter;
};

struct FbxVertex
{
	DXVector3 Pos;
	DXVector3 Normal;
	DXVector2 UV;
};
constexpr int GEOMETRYSIZE = sizeof(FbxVertex);

struct FBXModelData {

	FBXModelData();
	~FBXModelData();
	std::vector<FbxVertex> Data;	//�W�I���g���f�[�^

	//�O�p�`�|���S������`������W�I���g��3���擾����
	void GetPolygon(std::array<FbxVertex*, 3>&pOutPolygon,unsigned int pPolygonIndex);

	unsigned int PolygonCount;

	unsigned int PosLength;		//���_��
	unsigned int *Index;		//�C���f�b�N�X�o�b�t�@�����p
	unsigned int IndexLength;	//�C���f�b�N�X��

	
	std::weak_ptr<ColorChannel> Emissive;		//�G�~�b�V�u
	std::weak_ptr<ColorChannel> Ambient;		//�A���r�G���g
	std::weak_ptr<ColorChannel> Diffuse;		//�f�B�t���[�Y
	std::weak_ptr<ColorChannel> Specular;		//�X�y�L����



};
/*
	mesh:
		sub
		sub
	mesh:
		sub
	vector<FBXMesh>data;
	data[0].modeldata[0]

*/
//���b�V����̏��
struct FBXMesh {
	FBXMesh();
	~FBXMesh();
	//���b�V����Ɋ܂܂��T�u���b�V���̔z��
	std::vector<std::shared_ptr<FBXModelData>>subMesh;
	//���b�V���̍s��
	std::shared_ptr<D3DXMATRIX>mWorld;
//	D3DXMATRIX *mWorld;
};
/*
	�^�X�N�F
	���b�V���ƃT�u���b�V���̋��
	���b�V���̃O���[�o���|�W�V�����̓K�p
	HLSL�ɃM�����[�o���|�W�V������K�p����
	�������œK��
	�O������A�j���[�V�����̐ݒ�
*/