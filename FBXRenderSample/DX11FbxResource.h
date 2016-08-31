#pragma once
#include<d3dx9.h>
#include<vector>
struct ColorChannel {
	ColorChannel() {
		Color[0] = 0.0f;
		Color[1] = 0.0f;
		Color[2] = 0.0f;
		Color[3] = 1.0f;
	}
	std::string TextureName;
	float Color[4];
};

struct SimpleVertex
{
	D3DXVECTOR4 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};
constexpr int GEOMETRYSIZE = sizeof(SimpleVertex);

struct FBXModelData {

	std::vector<SimpleVertex> Data;	//�W�I���g���f�[�^

	unsigned int PosLength;
	unsigned int *Index;		//�C���f�b�N�X�o�b�t�@�����p
	unsigned int IndexLength;	//�C���f�b�N�X��

	ColorChannel *Emissive;		//�G�~�b�V�u
	ColorChannel *Ambient;		//�A���r�G���g
	ColorChannel *Diffuse;		//�f�B�t���[�Y
	ColorChannel *Specular;		//�X�y�L����



};
