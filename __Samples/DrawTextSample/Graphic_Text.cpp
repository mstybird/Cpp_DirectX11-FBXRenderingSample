//-----------------------------------------------------
//	DirectX テキスト描画サンプル
//	テキスト画像の作成、描画など
//	2010.03.04 TMO
//-----------------------------------------------------

#include "stdafx.h"
#include <locale.h>
#include "Graphic.h"

//作成されたD3Dデバイスを取得する
extern LPDIRECT3DDEVICE9 GetDevice();

//１文字のテキストを描いたテクスチャを作成する
void CreateTxtureChar(
	LPDIRECT3DTEXTURE9* pTexGet,
	int*	pTexWidGet,
	UINT	uChar,		//1文字のコード
	HFONT	hFont,
	DWORD	basecolor )
{
	LPDIRECT3DTEXTURE9	pTex	= NULL;
	BYTE*				ptr		= NULL;

	try {

		HDC		hdc		= GetDC(NULL);
		HFONT	oldFont	= (HFONT)SelectObject( hdc, hFont );

		//フォントビットマップ取得
		TEXTMETRIC TM;
		GetTextMetrics( hdc, &TM );
		GLYPHMETRICS GM;
		CONST MAT2 Mat = {{0,1},{0,0},{0,0},{0,1}};
		DWORD size = GetGlyphOutline( hdc, uChar, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat );
		ptr = new BYTE[size];

		if ( GDI_ERROR == GetGlyphOutline( hdc, uChar, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat ) ) {
			throw;
		}
		
		SelectObject( hdc, oldFont );
		ReleaseDC( NULL, hdc );

		//テクスチャ作成
		//文字に余白が入るので、テクスチャサイズはフォントサイズと異なる。
		if ( FAILED( GetDevice()->CreateTexture( GM.gmCellIncX, TM.tmHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTex, NULL ) ) ) {
		   if ( FAILED( GetDevice()->CreateTexture( GM.gmCellIncX, TM.tmHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTex, NULL ) ) ) {
			   throw;
		   }
		}

		D3DLOCKED_RECT LockedRect;
		if ( FAILED( pTex->LockRect( 0, &LockedRect, NULL, D3DLOCK_DISCARD ) ) ) {
			if ( FAILED( pTex->LockRect(0, &LockedRect, NULL, 0 ) ) ) {
				throw;
			}
		}

		//クリア
		ZeroMemory( LockedRect.pBits , LockedRect.Pitch * TM.tmHeight );

		//フォントビットマップの幅高
		int bmp_w = GM.gmBlackBoxX;
		int bmp_h = GM.gmBlackBoxY;
		//実際に確保されているビットマップの幅(4の倍数になってる)
		int bmp_pitch = GM.gmBlackBoxX + ( 4 - ( GM.gmBlackBoxX % 4 ) ) % 4;

		//文字左上の位置
		int ofs_x = GM.gmptGlyphOrigin.x;				
		int ofs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
	
		int px, py;
		DWORD Alpha, Color;

		for ( py = ofs_y; py < ofs_y + bmp_h; py++ ) {
			for ( px = ofs_x; px < ofs_x + bmp_w; px++ ) {
			  
				//アルファ値ではGGO_GRAY4_BITMAPが17階調(0～16)なので16で割る
				Alpha = (255 * ptr[px-ofs_x + bmp_pitch*(py-ofs_y)]) >> 4;
			  
				Color = basecolor | (Alpha << 24);

				memcpy( (BYTE*)LockedRect.pBits + LockedRect.Pitch*py + 4*px, &Color, sizeof( DWORD ) );
			}
		}
		pTex->UnlockRect(0);


		*pTexGet	= pTex;
		*pTexWidGet	= GM.gmCellIncX;//GM.gmCellIncXは指定したフォントサイズ通りにならないので、幅を別に覚えておく。

		//内容確認 透過で文字書いてるのでpngで保存
		D3DXSaveTextureToFileW((L"test.png"),D3DXIFF_PNG, pTex, NULL);

	} catch(...) {
		if ( pTex ) {
			pTex->Release();
		}
	}

	if ( ptr ) {
		delete[] ptr;
	}

}

TextuteTextLine::TextuteTextLine()
{
	m_align = AlignLeft;
	m_lineWidth = 0;
}

TextuteTextLine::~TextuteTextLine()
{
}


TextGraphic::TextGraphic()
{
	m_rect		= IRECT(0,0,0,0);
	m_fontsize	= 10;
	m_alphaColor= 0xffffffff;
}

TextGraphic::~TextGraphic()
{
	Release();
}

void TextGraphic::Release() {
	
	for ( std::vector< TextuteTextLine* >::iterator it = m_lineAry.begin(); it != m_lineAry.end(); it++ ) {
		
		std::vector<TxtureChar>::iterator it_char = (*it)->begin();
		for ( ; it_char != (*it)->end(); it_char++ ) {
			(*it_char).m_txtr->Release();
		}
		(*it)->clear();

		delete (*it);
	}
	m_lineAry.clear();

}

void TextGraphic::Create( LPCTSTR txt, int left, int top, int right, int bottom, LOGFONT& logFont )
{
	Release();

	m_rect.left   = left;
	m_rect.top    = top;
	m_rect.right  = right;
	m_rect.bottom = bottom;

	m_fontsize = logFont.lfHeight;
	
	HFONT hFont = CreateFontIndirect( &logFont );
	if( !hFont ) { return; }

	TCHAR* t = (TCHAR*)txt;

	TextuteTextLine* addLine = new TextuteTextLine();

	TxtureChar add;

#ifdef UNICODE
	unsigned int strnum = wcslen(txt);	//文字数
	int len = 1;						//１文字が使用している配列数
#else
	//マルチバイトの時
	setlocale( LC_CTYPE, "jpn" );			//ロケールの設定はmblen等を使うために必要ですが、アプリで一度だけ呼べば十分です。
	unsigned int strnum = _mbstrlen( txt );	//文字数
	int len;								//１文字が使用しているバイト数,マルチバイトなので変動します
#endif
	
	UINT code;

	for ( unsigned int c= 0; c < strnum; c++ )  {

		if ( *t == TEXT('\n') ) {

			m_lineAry.push_back( addLine );
			addLine = new TextuteTextLine();

			t++;
			continue;
		}
		
		//文字を作成
		#ifdef UNICODE
			//len = 1; //UNICODEの場合は常に1です
			code	= (UINT)( *t );
		#else
			//マルチバイトの時

			len = mblen( t, MB_CUR_MAX );//文字が使用するバイト数を取得します
			if ( len == 1 ) {
				//半角アルファベット等の場合 そのままUINTへ
				code = (UINT)( *t );
			} else
			if ( len == 2 ) {
				//漢字等の時は、文字2バイトを合成したUINTを作成します
				code = ( (BYTE)t[0] << 8 ) | (BYTE)t[1];
			}
		
		#endif

		//文字色は白で作っています。白色だとポリゴンのDiffuse色で半透明や色を操作できます。
		CreateTxtureChar( &add.m_txtr, &add.m_wid, code, hFont, 0x00ffffff );
		if ( addLine->m_lineWidth + add.m_wid > m_rect.Wid() ) {
			//折り返し
			m_lineAry.push_back( addLine );
			addLine = new TextuteTextLine();
		}

		addLine->push_back(add);
		addLine->m_lineWidth += add.m_wid;
		
		t += len;
	}

	if ( addLine->size() ) {
		m_lineAry.push_back( addLine );
	} else {
		delete addLine;
	}

	DeleteObject( hFont );

}

//行そろえの指定(全行)
void TextGraphic::SetTextAlign( TextAlign align )
{
	if ( m_lineAry.size() < 0 ){return;}
	
	for ( unsigned int c = 0; c <= m_lineAry.size()-1; c++ ) {
		m_lineAry[c]->m_align = align;
	}
}

//行そろえの指定(任意の複数行)
void TextGraphic::SetTextAlign(
	unsigned int lineIdx,	//開始位置
	unsigned int lineIdxTo,	//終了位置
	TextAlign align )
{
	if ( m_lineAry.size() >= lineIdx ) {
		return;
	}
	if ( m_lineAry.size() >= lineIdxTo ) {
		return;
	}
	for ( unsigned int c = lineIdx; c <= lineIdxTo; c++ ) {
		m_lineAry[c]->m_align = align;
	}
}

//テキストのアルファ値(描画するポリゴンのDiffuse色)変更
void TextGraphic::SetTextAlpha(DWORD alpha)
{
	m_alphaColor = alpha;
}

void TextGraphic::Draw()
{
	int st_x = m_rect.left;
	int st_y = m_rect.top;

	int cellwid;

	for ( std::vector< TextuteTextLine* >::iterator it = m_lineAry.begin(); it != m_lineAry.end(); it++ ) {
		st_x = m_rect.left;
		
		switch ( (*it)->m_align ) {
			case TextAlign::AlignLeft:
				break;
			case TextAlign::AlignRight:
				st_x += ( m_rect.Wid() - (*it)->m_lineWidth );
				break;
			case TextAlign::AlignCenter:
				st_x += ( m_rect.Wid() - (*it)->m_lineWidth ) >> 1;
				break;
		}

		for ( std::vector<TxtureChar>::iterator it2 = (*it)->begin(); it2 != (*it)->end(); it2++ ) {

			GetDevice()->SetTexture(0, (*it2).m_txtr );
			
			cellwid = (*it2).m_wid;
			GetGraphic()->Draw( (float)st_x, (float)st_y, (float)( st_x+cellwid ), (float)( st_y+m_fontsize ), m_alphaColor );
			st_x+=cellwid;

		}
		st_y += m_fontsize;
	}

}