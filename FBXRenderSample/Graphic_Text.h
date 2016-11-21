#pragma once
#include<vector>
#include<D3DX11.h>

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
	int GetHeigt() { return bottom - top; }

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
	using std::vector<TextureChar>::push_back;
	using std::vector<TextureChar>::size;
	using std::vector<TextureChar>::begin;
	using std::vector<TextureChar>::end;
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

	//テキスト画像作成
	void Create(LPCSTR aTxt, int aLeft, int aTop, int aRight, int aBottom, LOGFONT&aLogFont);
	//行揃えの指定
	void SetTextAlign(TextAlign aAlign);
	void SetTextAlign(uint32_t aLineIdx, uint32_t aLineIdxTo, TextAlign aAlign);
	//テキストのアルファ値(描画ポリゴンディフューズ変更)
	void SetTextAlpha(DWORD aAlpha);
	void Release();
	void Draw();
private:
	std::vector<TextureTextLine*>mLineAry;//行ごとのデータ
	IRECT mRect;	//描画範囲
	int mFontSize;	//フォントサイズ
	DWORD mAlphaColor;	//アルファ色(描画するポリゴンのディフューズ)
	static ID3D11Device *sDevice;
	static ID3D11DeviceContext *sDeviceContext;

};