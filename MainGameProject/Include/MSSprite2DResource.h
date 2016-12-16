#pragma once
#include<memory>
#include"DXMath.hpp"

//2D�`��p
//using std::weak_ptr;
//using std::unique_ptr;
#include<D3DX11.h>
class DXVector2;
class DXVector3;
class DXMatrix;
class DXTexture;
class DX11TextureManager;
class DXDisplay;
struct SpriteVertex;
class MSSpriteBaseResource abstract{
public:
	MSSpriteBaseResource();
	static void sInitialize(ID3D11Device*pDevice);
	//�`�悷��e�N�X�`���̐ݒ�
	void SetTexture(DX11TextureManager&pTextureManager, const int pID);
	void SetTexture(DXTexture*&pTexture);
	DXTexture* GetTexture();
	virtual void SetPosition(const DXVector2&pPosition);
	virtual void SetPosition(const DXVector3&pPosition);
	void SetRotation(const float aDegree);
	void AddRotation(const float aDegree);
	//�s�{�b�g��ݒ肷��
	void SetPivot(const DXVector2&pPivot);
	//�|���S���T�C�Y��ݒ肷��
	void SetSize(const DXVector2&pSize);
	//�g�嗦��ݒ肷��
	void SetScale(const DXVector2&pScale);
	//�摜�̃T�C�Y���܂߂Đ؂���
	void SetSplitSize(const float pLeft, const float pRight, const float pTop, const float pBottom);
	void SetSplitSizeX(const DXVector2&pSizeX);
	void SetSplitSizeY(const DXVector2&pSizeY);

	//�摜�T�C�Y�͂��̂܂܂Ő؂���
	void SetSplitImage(const float pLeft, const float pRight, const float pTop, const float pBottom);

	//�|���S������
	void CreateBuffer();

	const std::weak_ptr<DXVector3> GetPosition()const;
	const std::weak_ptr<DXVector2> GetPivot()const;
	const std::weak_ptr<DXVector2> GetSize()const;
	const std::weak_ptr<DXVector2> GetScale()const;
	const std::weak_ptr<DXMatrix>GetMatrix();
	DXMatrix GetMatrixWVP(DXDisplay& pDisplay);
	//���_�o�b�t�@�̍X�V���K�v�����ׂ�
	bool IsUpdateBuffering();

	//���_�o�b�t�@
	ID3D11Buffer*mVertexBuffer;
public:
	std::shared_ptr<DXVector3>mPosition;	//�`��ʒu
protected:
	//�|���S�������p
	virtual void CreatePolygon(SpriteVertex pPolygon[4]) = 0;
	//�`�悷��e�N�X�`��
	DXTexture* mTexture;
	//���̃C���X�^���X���e�N�X�`���̎��Ԃ�ێ����Ă��邩�ǂ���
	
	std::shared_ptr<DXVector2> mPivot;		//���S�ƂȂ�ʒu
	std::shared_ptr<DXVector2> mSize;		//�`��T�C�Y
	std::shared_ptr<DXVector2> mScale;		//�g�嗦
	float mRotation;						//��]�p�x(�f�B�O���[)
	DXVector2 mSplitPolygonX;				//�|���S���̐؂���(����)
	DXVector2 mSplitPolygonY;				//�|���S���̐؂���(����)
	DXVector2 mSplitImageX;					//�C�}�[�W�̐؂���(����)
	DXVector2 mSplitImageY;					//�C�}�[�W�̐؂���(����)
	std::shared_ptr<DXMatrix> mMatrix;		//�ʒu�A�g�嗦����������p�̍s��
	bool mUpdateFlag;	//�|���S���Đ������K�v�ȕύX���������ꍇ��true�ƂȂ�
	static ID3D11Device* sDevice;
};

//�X�N���[����ԕ`��p
class MSSprite2DResource :public MSSpriteBaseResource {
public:
	void SetPosition(const DXVector2&pPosition)override;
	void SetPosition(const DXVector3&pPosition)override ;
private:
	void CreatePolygon(SpriteVertex pPolygon[4])override;

};
//3D��ԕ`��p���\�[�X
class MSSprite3DResource :public MSSpriteBaseResource {
public:
	void CreatePolygon(SpriteVertex pPolygon[4])override;

};