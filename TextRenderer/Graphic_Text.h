#pragma once
#include<vector>
#include<D3DX11.h>


struct IRECT {
	int left;
	int top;
	int right;
	int bottom;
};

struct TextureChar {
	ID3D11SamplerState* m_pSampleLinear;//�e�N�X�`���[�̃T���v���[
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[
	int mWidth;	//������
};

//�s����
enum TextAlign {
	AlignLeft,
	AlignRight,
	AlignCenter,
};

//TextGraphic��������s���̃e�L�X�g
class TextureTextLine :private std::vector<TextureChar> {
public:
	TextAlign mAlign;	//�s����
	int mWidth;			//1�s�̕�

	TextureTextLine();
	~TextureTextLine();

};

//Graphic�@�\�������e�L�X�g�f�[�^
class TextuGraphic {
private:
	std::vector<TextureTextLine*>mLineAry;//�s���Ƃ̃f�[�^
	IRECT mRect;	//�`��͈�
	int mFontSize;	//�t�H���g�T�C�Y
	DWORD mAlphaColor;	//�A���t�@�F(�`�悷��|���S���̃f�B�t���[�Y)
public:
	TextuGraphic();
	virtual ~TextuGraphic();

	//�e�L�X�g�摜�쐬
	void Create(LPCSTR aTxt, int aLeft, int aTop, int aRight, int aBottom, LOGFONT&aLogFont);
	//�s�����̎w��
	void SetTextAlign(uint32_t aLineIdx, uint32_t aLineIdxTo, TextAlign aAlign);
	//�e�L�X�g�̃A���t�@�l(�`��|���S���f�B�t���[�Y�ύX)
	void SetTextAlpha(DWORD aAlpha);
	void Release();
	void Draw();

};