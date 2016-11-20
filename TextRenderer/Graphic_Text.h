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
	ID3D11SamplerState* m_pSampleLinear;//テクスチャーのサンプラー
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー
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
	TextAlign mAlign;	//行揃え
	int mWidth;			//1行の幅

	TextureTextLine();
	~TextureTextLine();

};

//Graphic機能が扱うテキストデータ
class TextuGraphic {
private:
	std::vector<TextureTextLine*>mLineAry;//行ごとのデータ
	IRECT mRect;	//描画範囲
	int mFontSize;	//フォントサイズ
	DWORD mAlphaColor;	//アルファ色(描画するポリゴンのディフューズ)
public:
	TextuGraphic();
	virtual ~TextuGraphic();

	//テキスト画像作成
	void Create(LPCSTR aTxt, int aLeft, int aTop, int aRight, int aBottom, LOGFONT&aLogFont);
	//行揃えの指定
	void SetTextAlign(uint32_t aLineIdx, uint32_t aLineIdxTo, TextAlign aAlign);
	//テキストのアルファ値(描画ポリゴンディフューズ変更)
	void SetTextAlpha(DWORD aAlpha);
	void Release();
	void Draw();

};