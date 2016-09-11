#pragma once
#include<memory>
//#include"DXVector2.h"

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
	void SetTexture(const std::weak_ptr<DX11TextureManager>&pTextureManager, const int pID);
	std::weak_ptr<DXTexture> GetTexture();
	virtual void SetPosition(const DXVector2&pPosition);
	virtual void SetPosition(const DXVector3&pPosition);
	void SetPivot(const DXVector2&pPivot);
	void SetSize(const DXVector2&pSize);
	void SetScale(const DXVector2&pScale);
	const std::weak_ptr<DXVector3> GetPosition()const;
	const std::weak_ptr<DXVector2> GetPivot()const;
	const std::weak_ptr<DXVector2> GetSize()const;
	const std::weak_ptr<DXVector2> GetScale()const;
	const std::weak_ptr<DXMatrix>GetMatrix();
	std::unique_ptr<DXMatrix> GetMatrixWVP(const std::weak_ptr<DXDisplay>pDisplay);
	//���_�o�b�t�@
	ID3D11Buffer*mVertexBuffer;
protected:
	//�s�{�b�g�A�T�C�Y���ύX���ꂽ�ۂɃ|���S�����Đ�������
	void CreateBuffer();
	virtual void CreatePolygon(SpriteVertex pPolygon[4]) = 0;;
	//�`�悷��e�N�X�`��
	std::weak_ptr<DXTexture> mTexture;
	
	std::shared_ptr<DXVector3>mPosition;	//�`��ʒu
	std::shared_ptr<DXVector2> mPivot;		//���S�ƂȂ�ʒu
	std::shared_ptr<DXVector2> mSize;		//�`��T�C�Y
	std::shared_ptr<DXVector2> mScale;		//�g�嗦
	std::shared_ptr<DXMatrix> mMatrix;		//�ʒu�A�g�嗦����������p�̍s��
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