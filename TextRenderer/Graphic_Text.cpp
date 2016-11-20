#include "Graphic_Text.h"
#include<locale>
TextureTextLine::TextureTextLine()
{
	mAlign = AlignLeft;
	mWidth = 0;
}

TextureTextLine::~TextureTextLine()
{
}

TextuGraphic::TextuGraphic()
{
	mRect = { 0,0,0,0 };
	mFontSize = 10;
	mAlphaColor = 0xFFFF'FFFF;
}

TextuGraphic::~TextuGraphic()
{
	Release();
}

void TextuGraphic::Create(LPCSTR aTxt, int aLeft, int aTop, int aRight, int aBottom, LOGFONT & aLogFont)
{
	Release();
	mRect.left = aLeft;
	mRect.top = aTop;
	mRect.right = aRight;
	mRect.bottom = aBottom;
	mFontSize = aLogFont.lfHeight;
	HFONT lFont = CreateFontIndirect(&aLogFont);
	if (!lFont)return;

	TCHAR* t = (TCHAR*)aTxt;
	TextureTextLine* lAddLine = new TextureTextLine();
	TextureChar lAdd;
#ifdef UNICODE
	uint32_t lStrNum = wcslen(aTxt);
	int lLen = 1;	//1�������g�p���Ă���z��
#else
	//�}���`�o�C�g�̎�
	setlocale(LC_CTYPE, "jpn");
	uint32_t lStrNum = _mbstrlen(aTxt);
	int lLen;	//�}���`�o�C�g�̂Ƃ���1�����̃o�C�g���͕ϓ�����
#endif
}
