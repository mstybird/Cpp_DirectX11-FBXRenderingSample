//-----------------------------------------------------
//	DirectX �e�L�X�g�`��T���v��
//	�e�N�X�`���̍쐬�Ȃ�
//	2010.03.04 TMO
//-----------------------------------------------------

#include "stdafx.h"
#include "Graphic.h"

//�쐬���ꂽD3D�f�o�C�X���擾����
extern LPDIRECT3DDEVICE9 GetDevice();

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
	Release();
}

//�e�N�X�`���̃��[�h
bool CTexture::Load( LPCTSTR path )
{
	HRESULT hr = D3DXCreateTextureFromFile( GetDevice(), path, &m_texture );
	if ( FAILED(hr) ) {
		return false;
	}
	return true;
}

//�`��Ɏg���e�N�X�`�����f�o�C�X�ɃZ�b�g����
void CTexture::SetCurrentUse()
{
	GetDevice()->SetTexture(0, m_texture );
}

//�e�N�X�`�����������
void CTexture::Release()
{
	if ( m_texture != NULL ) {
		m_texture->Release();
		m_texture = NULL;
	}
}
