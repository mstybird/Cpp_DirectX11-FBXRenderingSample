#pragma once
#include<memory>
#include"DX11TextureManager.h"
#include"DXVector2.h"
#include"DXMatrix.h"
//2D�`��p
//using std::weak_ptr;
//using std::unique_ptr;
#include<D3DX11.h>
class MSSprite2DResource {
public:
	MSSprite2DResource();
	static void sInitialize(ID3D11Device*pDevice);
	//�`�悷��e�N�X�`���̐ݒ�
	void SetTexture(const std::weak_ptr<DX11TextureManager>&pTextureManager, const int pID);
	std::weak_ptr<DXTexture> GetTexture();
	void SetPosition(const DXVector2&pPosition);
	void SetPivot(const DXVector2&pPivot);
	void SetSize(const DXVector2&pSize);
	void SetScale(const DXVector2&pScale);
	const std::weak_ptr<DXVector2> GetPosition()const;
	const std::weak_ptr<DXVector2> GetPivot()const;
	const std::weak_ptr<DXVector2> GetSize()const;
	const std::weak_ptr<DXVector2> GetScale()const;
	const std::weak_ptr<DXMatrix>GetMatrix();
	//���_�o�b�t�@
	ID3D11Buffer*mVertexBuffer;
private:
	//�`�悷��e�N�X�`��
	std::weak_ptr<DXTexture> mTexture;
	
	std::shared_ptr<DXVector2>mPosition;	//�`��ʒu
	std::shared_ptr<DXVector2> mPivot;		//���S�ƂȂ�ʒu
	std::shared_ptr<DXVector2> mSize;		//�`��T�C�Y
	std::shared_ptr<DXVector2> mScale;		//�g�嗦
	std::shared_ptr<DXMatrix> mMatrix;		//�ʒu�A�g�嗦����������p�̍s��
	static ID3D11Device* sDevice;
};

