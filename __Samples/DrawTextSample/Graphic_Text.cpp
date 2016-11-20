//-----------------------------------------------------
//	DirectX �e�L�X�g�`��T���v��
//	�e�L�X�g�摜�̍쐬�A�`��Ȃ�
//	2010.03.04 TMO
//-----------------------------------------------------

#include "stdafx.h"
#include <locale.h>
#include "Graphic.h"

//�쐬���ꂽD3D�f�o�C�X���擾����
extern LPDIRECT3DDEVICE9 GetDevice();

//�P�����̃e�L�X�g��`�����e�N�X�`�����쐬����
void CreateTxtureChar(
	LPDIRECT3DTEXTURE9* pTexGet,
	int*	pTexWidGet,
	UINT	uChar,		//1�����̃R�[�h
	HFONT	hFont,
	DWORD	basecolor )
{
	LPDIRECT3DTEXTURE9	pTex	= NULL;
	BYTE*				ptr		= NULL;

	try {

		HDC		hdc		= GetDC(NULL);
		HFONT	oldFont	= (HFONT)SelectObject( hdc, hFont );

		//�t�H���g�r�b�g�}�b�v�擾
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

		//�e�N�X�`���쐬
		//�����ɗ]��������̂ŁA�e�N�X�`���T�C�Y�̓t�H���g�T�C�Y�ƈقȂ�B
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

		//�N���A
		ZeroMemory( LockedRect.pBits , LockedRect.Pitch * TM.tmHeight );

		//�t�H���g�r�b�g�}�b�v�̕���
		int bmp_w = GM.gmBlackBoxX;
		int bmp_h = GM.gmBlackBoxY;
		//���ۂɊm�ۂ���Ă���r�b�g�}�b�v�̕�(4�̔{���ɂȂ��Ă�)
		int bmp_pitch = GM.gmBlackBoxX + ( 4 - ( GM.gmBlackBoxX % 4 ) ) % 4;

		//��������̈ʒu
		int ofs_x = GM.gmptGlyphOrigin.x;				
		int ofs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
	
		int px, py;
		DWORD Alpha, Color;

		for ( py = ofs_y; py < ofs_y + bmp_h; py++ ) {
			for ( px = ofs_x; px < ofs_x + bmp_w; px++ ) {
			  
				//�A���t�@�l�ł�GGO_GRAY4_BITMAP��17�K��(0�`16)�Ȃ̂�16�Ŋ���
				Alpha = (255 * ptr[px-ofs_x + bmp_pitch*(py-ofs_y)]) >> 4;
			  
				Color = basecolor | (Alpha << 24);

				memcpy( (BYTE*)LockedRect.pBits + LockedRect.Pitch*py + 4*px, &Color, sizeof( DWORD ) );
			}
		}
		pTex->UnlockRect(0);


		*pTexGet	= pTex;
		*pTexWidGet	= GM.gmCellIncX;//GM.gmCellIncX�͎w�肵���t�H���g�T�C�Y�ʂ�ɂȂ�Ȃ��̂ŁA����ʂɊo���Ă����B

		//���e�m�F ���߂ŕ��������Ă�̂�png�ŕۑ�
		//D3DXSaveTextureToFileW(TEXT("c:\\test.png"),D3DXIFF_PNG, pTex, NULL);

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
	unsigned int strnum = wcslen(txt);	//������
	int len = 1;						//�P�������g�p���Ă���z��
#else
	//�}���`�o�C�g�̎�
	setlocale( LC_CTYPE, "jpn" );			//���P�[���̐ݒ��mblen�����g�����߂ɕK�v�ł����A�A�v���ň�x�����ĂׂΏ\���ł��B
	unsigned int strnum = _mbstrlen( txt );	//������
	int len;								//�P�������g�p���Ă���o�C�g��,�}���`�o�C�g�Ȃ̂ŕϓ����܂�
#endif
	
	UINT code;

	for ( unsigned int c= 0; c < strnum; c++ )  {

		if ( *t == TEXT('\n') ) {

			m_lineAry.push_back( addLine );
			addLine = new TextuteTextLine();

			t++;
			continue;
		}
		
		//�������쐬
		#ifdef UNICODE
			//len = 1; //UNICODE�̏ꍇ�͏��1�ł�
			code	= (UINT)( *t );
		#else
			//�}���`�o�C�g�̎�

			len = mblen( t, MB_CUR_MAX );//�������g�p����o�C�g�����擾���܂�
			if ( len == 1 ) {
				//���p�A���t�@�x�b�g���̏ꍇ ���̂܂�UINT��
				code = (UINT)( *t );
			} else
			if ( len == 2 ) {
				//�������̎��́A����2�o�C�g����������UINT���쐬���܂�
				code = ( (BYTE)t[0] << 8 ) | (BYTE)t[1];
			}
		
		#endif

		//�����F�͔��ō���Ă��܂��B���F���ƃ|���S����Diffuse�F�Ŕ�������F�𑀍�ł��܂��B
		CreateTxtureChar( &add.m_txtr, &add.m_wid, code, hFont, 0x00ffffff );
		if ( addLine->m_lineWidth + add.m_wid > m_rect.Wid() ) {
			//�܂�Ԃ�
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

//�s���낦�̎w��(�S�s)
void TextGraphic::SetTextAlign( TextAlign align )
{
	if ( m_lineAry.size() < 0 ){return;}
	
	for ( unsigned int c = 0; c <= m_lineAry.size()-1; c++ ) {
		m_lineAry[c]->m_align = align;
	}
}

//�s���낦�̎w��(�C�ӂ̕����s)
void TextGraphic::SetTextAlign(
	unsigned int lineIdx,	//�J�n�ʒu
	unsigned int lineIdxTo,	//�I���ʒu
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

//�e�L�X�g�̃A���t�@�l(�`�悷��|���S����Diffuse�F)�ύX
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