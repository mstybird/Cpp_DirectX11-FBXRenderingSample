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
	friend class TextManager;

public:



	TextGraphic();
	virtual ~TextGraphic();

	static void SetDevice(ID3D11Device*&aDevice);
	static void SetDeviceContext(ID3D11DeviceContext*&aDeviceContext);

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
class FontLog :public LOGFONT{
private:
	using LOGFONT::lfHeight;
public:
	void SetFontSize(long aHeight) {
		lfHeight = aHeight;
	}
	long GetFontSize()const {
		return lfHeight;
	}
};

struct FontDesc {
	int Width;
	int Height;
};

enum class RegisterFlag {
	NoRegist,
	Registerd
};

struct FontResource {
	//�t�H���g���
	FontLog Desc;
	//�_���t�H���g
	HFONT Font;
};



//�_���t�H���g���Ǘ�����
class FontDatabase {
public:
	FontDatabase();
	~FontDatabase();

	//FontDesc���g���Ę_���t�H���g�̍쐬
	bool Load(const FontLog&aFontDescription,const int aID);
	//�o�^�ςݘ_���t�H���g�����ׂĉ��
	void Release();
	//�_���t�H���g��L��������
	bool Enable(const int aID);
	//�A�N�e�B�u�Ș_���t�H���g�𖳌�������
	bool Disable();
	//�o�^�ςݘ_���t�H���g�����݂��邩
	bool IsExist(const int aID);
	//���ݗL��������Ă���t�H���g��ID���擾����
	int GetActiveFontID();
	//���ݗL��������Ă���t�H���g�̏����擾����
	FontLog* GetActiveFontDesc();
	//���ݗL��������Ă���_���t�H���g���擾����
	HFONT* GetActiveFontHFont();
private:
	//ID,�t�H���g���
	std::unordered_map<int, FontResource>mDb;
	//�A�N�e�B�u�ȃt�H���g��ID,-1�Ŗ���
	int mActiveID;
};


using CharTextureManager = std::unordered_map<int, std::unordered_map<int, DX11TextureManager>>;
using CharDescManager=std::unordered_map<int, std::unordered_map<int, FontDesc>>;

using CharResource = std::pair<DXTexture*, FontDesc*>;

//�ǂݍ��񂾃e�L�X�g�e�N�X�`�����L��(�L���b�V��)����
//�t�H���g��ʂɓo�^�ł���
class CharDatabase{
public:

	static void SetDevice(ID3D11Device*&aDevice);
	static void SetDeviceContext(ID3D11DeviceContext*&aDeviceContext);
	//��������܂Ƃ߂ēǂݍ���
	bool Load(HDC&aHandle,const std::string& aString,const int aFontID, const int aFontSize);
	//1�����ǂݍ���
	bool Load(HDC&aHandle,const uint32_t aCharCode, const int aFontID, const int aFontSize);
	//�e�N�X�`���̉��
	void Release();
	//�w�肵���t�H���g�̂݉��
	void Release(const int aFontID, const int aFontSize);
	//�w�肵�������̃f�[�^���擾����
	CharResource Get(const uint32_t aCharCode, const int aFontID, const int aFontSize);
	//������̃f�[�^���擾����
	std::vector<CharResource> GetArray(const std::string&aText, const int aFontID, const int aFontSize);

	//�e�N�X�`�������݂��邩
	bool IsExist(const uint32_t aCharCode, const int aFontID, const int aFontSize);

	//�����񂷂ׂĂ̕����R�[�h�̔z����擾
	static std::vector<uint32_t> GetCharCodeArray(const std::string&aText);

private:

	void RegisterTexture(const int aFontID, const int aFontSize, const uint32_t aCharCode, ID3D11Texture2D*& aTexture);
	void RegisterDescription(const int aFontID, const int aFontSize, const uint32_t aCharCode, const int aWidth, const int aHeight);

	static ID3D11Device *sDevice;
	static ID3D11DeviceContext *sDeviceContext;

	//FontID,FontSize,Manager
	CharTextureManager mTextureManager;
	CharDescManager mDescManager;
};

//�_���t�H���g�A�e�L�X�g�e�N�X�`���̊Ǘ��A
//TextGraphic�̐������s��
class TextManager {
public:
	TextManager();
	~TextManager();

	static void SetDevice(ID3D11Device*&aDevice);
	static void SetDeviceContext(ID3D11DeviceContext*&aDeviceContext);
	//�t�H���g�̓o�^
	bool RegisterFont(const FontLog&aFontDescription,const int aID);
	//�e�L�X�g�e�N�X�`���̃L���b�V�����쐬
	bool PreLoadString(const std::string&aText);
	//����쐬����e�L�X�g
	bool SetFont(const int aID);
	//�f�t�H���g�̕`��͈͂��w��
	void SetDefaultSize(const int aWidth, const int aHeight);
	//
	std::shared_ptr<TextGraphic> Create(const std::string& aText);

private:
	static ID3D11Device *sDevice;
	static ID3D11DeviceContext *sDeviceContext;

	std::unique_ptr<FontDatabase> mDBFont;
	std::unique_ptr<CharDatabase> mDBChar;
	DXVector2 mSize;
};
