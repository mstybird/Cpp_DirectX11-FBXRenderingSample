#pragma once
#include<vector>
#include<D3DX11.h>
#include"MSSprite2DResource.h"
#include"MySprite2DShader.h"
#include"MSSprite2DRender.h"
#include<DX11TextureManager.hpp>
//�e�N�X�`����A8R8G8B8��32�r�b�g�Ƃ���

struct IRECT {
	int left;
	int top;
	int right;
	int bottom;
	IRECT() { left = top = right = bottom = 0; }
	IRECT(int l, int t, int r, int b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
	int GetWidth() { return right - left; }
	int GetHeight() { return bottom - top; }

};

struct TextureChar:public MSSpriteBaseResource {
	//�e�L�X�g�`����s���͈͂̃|���S���𐶐�
	virtual void CreatePolygon(SpriteVertex pPolygon[4])override;
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
	using std::vector<TextureChar>::push_back;
	using std::vector<TextureChar>::size;
	using std::vector<TextureChar>::begin;
	using std::vector<TextureChar>::end;
	using std::vector<TextureChar>::back;
	using std::vector<TextureChar>::clear;
	TextAlign mAlign;	//�s����
	int mWidth;			//1�s�̕�

	TextureTextLine();
	~TextureTextLine();

};

//Graphic�@�\�������e�L�X�g�f�[�^
class TextGraphic {
private:
	void CreateTextureChar(
		TextureChar& aTexture,
		int& aTexWidGet,
		uint32_t aUChar,
		HFONT aFont,
		DWORD aBaseColor
	);
public:



	TextGraphic();
	virtual ~TextGraphic();

	static void SetDevice(ID3D11Device*&aDevice);
	static void SetDeviceContext(ID3D11DeviceContext*&aDeviceContext);

	//�e�L�X�g�摜�쐬
	void Create(LPCSTR aTxt, int aLeft, int aTop, int aRight, int aBottom, LOGFONT&aLogFont);

	//�s�����̎w��
	void SetTextAlign(TextAlign aAlign);
	void SetTextAlign(uint32_t aLineIdx, uint32_t aLineIdxTo, TextAlign aAlign);
	//�e�L�X�g�̃A���t�@�l(�`��|���S���f�B�t���[�Y�ύX)
	void SetTextAlpha(DWORD aAlpha);
	void Release();
	void Render(MSSprite2DRender&aRender);

private:
	std::vector<TextureTextLine*>mLineAry;//�s���Ƃ̃f�[�^
	IRECT mRect;	//�`��͈�
	int mFontSize;	//�t�H���g�T�C�Y
	DWORD mAlphaColor;	//�A���t�@�F(�`�悷��|���S���̃f�B�t���[�Y)
	static ID3D11Device *sDevice;
	static ID3D11DeviceContext *sDeviceContext;

	//�����e�N�X�`���}�l�[�W��
	DX11TextureManager mTextManager;

};

/*
	�e�L�X�g�֌W�͂������琶��������悤�ɂ���
*/

//LOGFONT�̊g���B
class FontDesc :public LOGFONT{
};

//�_���t�H���g���Ǘ�����
class FontDatabase {

};

//�ǂݍ��񂾃e�L�X�g�e�N�X�`�����L��(�L���b�V��)����
class CharDatabase {

};

//�_���t�H���g�A�e�L�X�g�e�N�X�`���̊Ǘ��A
//TextGraphic�̐������s��
class TextManager {

};
