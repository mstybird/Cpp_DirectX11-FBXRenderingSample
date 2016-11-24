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
	friend class TextManager;

public:



	TextGraphic();
	virtual ~TextGraphic();

	static void SetDevice(ID3D11Device*&aDevice);
	static void SetDeviceContext(ID3D11DeviceContext*&aDeviceContext);

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
class FontLog :public LOGFONT{
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
	//フォント情報
	FontLog Desc;
	//論理フォント
	HFONT Font;
};



//論理フォントを管理する
class FontDatabase {
public:
	FontDatabase();
	~FontDatabase();

	//FontDescを使って論理フォントの作成
	bool Load(const FontLog&aFontDescription,const int aID);
	//登録済み論理フォントをすべて解放
	void Release();
	//論理フォントを有効化する
	bool Enable(const int aID);
	//アクティブな論理フォントを無効化する
	bool Disable();
	//登録済み論理フォントが存在するか
	bool IsExist(const int aID);
	//現在有効化されているフォントのIDを取得する
	int GetActiveFontID();
	//現在有効化されているフォントの情報を取得する
	FontLog* GetActiveFontDesc();
	//現在有効化されている論理フォントを取得する
	HFONT* GetActiveFontHFont();
private:
	//ID,フォント情報
	std::unordered_map<int, FontResource>mDb;
	//アクティブなフォントのID,-1で無効
	int mActiveID;
};


using CharTextureManager = std::unordered_map<int, std::unordered_map<int, DX11TextureManager>>;
using CharDescManager=std::unordered_map<int, std::unordered_map<int, FontDesc>>;

using CharResource = std::pair<DXTexture*, FontDesc*>;

//読み込んだテキストテクスチャを記憶(キャッシュ)する
//フォント種別に登録できる
class CharDatabase{
public:

	static void SetDevice(ID3D11Device*&aDevice);
	static void SetDeviceContext(ID3D11DeviceContext*&aDeviceContext);
	//文字列をまとめて読み込み
	bool Load(HDC&aHandle,const std::string& aString,const int aFontID, const int aFontSize);
	//1文字読み込み
	bool Load(HDC&aHandle,const uint32_t aCharCode, const int aFontID, const int aFontSize);
	//テクスチャの解放
	void Release();
	//指定したフォントのみ解放
	void Release(const int aFontID, const int aFontSize);
	//指定した文字のデータを取得する
	CharResource Get(const uint32_t aCharCode, const int aFontID, const int aFontSize);
	//文字列のデータを取得する
	std::vector<CharResource> GetArray(const std::string&aText, const int aFontID, const int aFontSize);

	//テクスチャが存在するか
	bool IsExist(const uint32_t aCharCode, const int aFontID, const int aFontSize);

	//文字列すべての文字コードの配列を取得
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

//論理フォント、テキストテクスチャの管理、
//TextGraphicの生成を行う
class TextManager {
public:
	TextManager();
	~TextManager();

	static void SetDevice(ID3D11Device*&aDevice);
	static void SetDeviceContext(ID3D11DeviceContext*&aDeviceContext);
	//フォントの登録
	bool RegisterFont(const FontLog&aFontDescription,const int aID);
	//テキストテクスチャのキャッシュを作成
	bool PreLoadString(const std::string&aText);
	//今後作成するテキスト
	bool SetFont(const int aID);
	//デフォルトの描画範囲を指定
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
