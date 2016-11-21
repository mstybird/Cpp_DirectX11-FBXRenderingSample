#pragma once
#include<vector>
#include<D3DX11.h>

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
	int GetHeigt() { return bottom - top; }

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
	using std::vector<TextureChar>::push_back;
	using std::vector<TextureChar>::size;
	using std::vector<TextureChar>::begin;
	using std::vector<TextureChar>::end;
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
		ID3D11ShaderResourceView*& aTexture,
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
	void Draw();
private:
	std::vector<TextureTextLine*>mLineAry;//�s���Ƃ̃f�[�^
	IRECT mRect;	//�`��͈�
	int mFontSize;	//�t�H���g�T�C�Y
	DWORD mAlphaColor;	//�A���t�@�F(�`�悷��|���S���̃f�B�t���[�Y)
	static ID3D11Device *sDevice;
	static ID3D11DeviceContext *sDeviceContext;

};