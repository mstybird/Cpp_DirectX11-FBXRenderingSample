//-----------------------------------------------------
//	DirectX テキスト描画サンプル
//	テクスチャの作成など
//	2010.03.04 TMO
//-----------------------------------------------------

#include "stdafx.h"
#include "Graphic.h"

//作成されたD3Dデバイスを取得する
extern LPDIRECT3DDEVICE9 GetDevice();

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
	Release();
}

//テクスチャのロード
bool CTexture::Load( LPCTSTR path )
{
	HRESULT hr = D3DXCreateTextureFromFile( GetDevice(), path, &m_texture );
	if ( FAILED(hr) ) {
		return false;
	}
	return true;
}

//描画に使うテクスチャをデバイスにセットする
void CTexture::SetCurrentUse()
{
	GetDevice()->SetTexture(0, m_texture );
}

//テクスチャを解放する
void CTexture::Release()
{
	if ( m_texture != NULL ) {
		m_texture->Release();
		m_texture = NULL;
	}
}
