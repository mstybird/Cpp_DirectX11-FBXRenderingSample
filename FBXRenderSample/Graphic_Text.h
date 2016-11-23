#pragma once
#include<vector>
#include<D3DX11.h>
#include"MSSprite2DResource.h"
#include"MySprite2DShader.h"
#include"MSSprite2DRender.h"
#include<DX11TextureManager.hpp>
//テクスチャはA8R8G8B8の32ビットとする

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
	//テキスト描画を行う範囲のポリゴンを生成
	virtual void CreatePolygon(SpriteVertex pPolygon[4])override;
	int mWidth;	//文字幅
};

//行揃え
enum TextAlign {
	AlignLeft,
	AlignRight,
	AlignCenter,
};

//TextGraphicが扱う一行分のテキスト
class TextureTextLine :private std::vector<TextureChar> {
public:
	using std::vector<TextureChar>::push_back;
	using std::vector<TextureChar>::size;
	using std::vector<TextureChar>::begin;
	using std::vector<TextureChar>::end;
	using std::vector<TextureChar>::back;
	using std::vector<TextureChar>::clear;
	TextAlign mAlign;	//行揃え
	int mWidth;			//1行の幅

	TextureTextLine();
	~TextureTextLine();

};

//Graphic機能が扱うテキストデータ
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

	//テキスト画像作成
	void Create(LPCSTR aTxt, int aLeft, int aTop, int aRight, int aBottom, LOGFONT&aLogFont);

	//行揃えの指定
	void SetTextAlign(TextAlign aAlign);
	void SetTextAlign(uint32_t aLineIdx, uint32_t aLineIdxTo, TextAlign aAlign);
	//テキストのアルファ値(描画ポリゴンディフューズ変更)
	void SetTextAlpha(DWORD aAlpha);
	void Release();
	void Render(MSSprite2DRender&aRender);

private:
	std::vector<TextureTextLine*>mLineAry;//行ごとのデータ
	IRECT mRect;	//描画範囲
	int mFontSize;	//フォントサイズ
	DWORD mAlphaColor;	//アルファ色(描画するポリゴンのディフューズ)
	static ID3D11Device *sDevice;
	static ID3D11DeviceContext *sDeviceContext;

	//文字テクスチャマネージャ
	DX11TextureManager mTextManager;

};

/*
	テキスト関係はここから生成させるようにする
*/

//LOGFONTの拡張。
class FontDesc :public LOGFONT{
};

//論理フォントを管理する
class FontDatabase {

};

//読み込んだテキストテクスチャを記憶(キャッシュ)する
class CharDatabase {

};

//論理フォント、テキストテクスチャの管理、
//TextGraphicの生成を行う
class TextManager {

};
