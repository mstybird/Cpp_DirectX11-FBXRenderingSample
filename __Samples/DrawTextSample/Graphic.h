//-----------------------------------------------------
//	DirectX テキスト描画サンプル
//	Direct3Dの描画関連
//	2010.03.04 TMO
//-----------------------------------------------------

#pragma once
#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9tex.h>

#pragma warning(disable:4482)

struct FPOINT
{
	float x;
	float y;

	FPOINT()
	{
	}
	FPOINT( float fx, float fy )
	{
		x = fx;
		y = fy;
	}
	
	FPOINT& operator + (const FPOINT& src ) {
		x += src.x;
		y += src.y;
		return *this;
	}

};

struct FRECT
{
	float left;
	float top;
	float right;
	float bottom;

	FRECT(){ left = top = right = bottom = 0; }
	FRECT(float l, float t, float r, float b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
	float Wid(){return ( right - left );}
	float Hgt(){return ( bottom - top );}

	void SetWid(float w) {
		
		left = left + ( ( right - left ) - w ) * 0.5f;
		
		right = left+w;
	}
	void SetHgt(float h) {
		top = top + ( ( bottom - top ) - h ) * 0.5f;
		bottom = top+h;
	}

	void SetCenter( const FPOINT& fp ) {
		//TOFO
		float w = Wid();
		float h = Hgt();
		left   = fp.x - ( w / 2 );
		right  = left + w;
		top    = fp.y - ( h / 2 );
		bottom = top +h;
	}
	void SetCenter( float x, float y ) {
		//TOFO
		float w = Wid();
		float h = Hgt();
		left   = x - ( w / 2 );
		right  = left + w;
		top    = y - ( h / 2 );
		bottom = top +h;
	}
	
	void Move( float x, float y ) {
		left   += x;
		top    += y;
		right  += x;
		bottom += y;
	}

	FPOINT Center()
	{
		return FPOINT( left + ( ( right - left ) * 0.5f ) , top + ( ( bottom - top )*0.5f ) );
	}
};
struct IRECT
{
	int left;
	int top;
	int right;
	int bottom;

	IRECT(){ left = top = right = bottom = 0; }
	IRECT(int l, int t, int r, int b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
	int Wid() {return right - left;}
	int Hgt() {return bottom - top;}
};

//テクスチャクラス
class CTexture {
	LPDIRECT3DTEXTURE9	m_texture;
public:
	CTexture();
	~CTexture();
	
	bool Load( LPCTSTR path );
	void SetCurrentUse();
	void Release();
};

//描画関連クラス
class CGraphic
{
	enum DisplayMode {
		FullScreen,
		WindowMode,
	};

	DisplayMode	m_currDisplayMode;//画面モード

public :
	DWORD		m_backColor;//背景色

	CGraphic();
	~CGraphic();

	//生成と破棄
	bool Create(HWND hWnd);
	void Destroy();

	void ClearAndBegin();//Draw前に呼ぶ
	void EndAndPresent();//Draw後に呼ぶ

	void DrawColorRect( float left, float top, float right, float bottom, DWORD color );
	void Draw( float left, float top, float right, float bottom, DWORD alphaCol );
	void Draw( float left, float top, float right, float bottom, CTexture* texture, DWORD alphaCol );
};

//テキスト関連
#include "Graphic_Text.h"


//CGraphicクラスの生成と破棄はこれで行う。
CGraphic*	CreateGraphic(HWND hWnd);
CGraphic*	GetGraphic();
void		ReleaseGraphic();