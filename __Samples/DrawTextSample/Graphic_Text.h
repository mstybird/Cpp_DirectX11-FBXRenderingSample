//-----------------------------------------------------
//	DirectX テキスト描画サンプル
//	テキスト画像の作成、描画など
//	2010.03.04 TMO
//-----------------------------------------------------
#pragma once

//TextGraphicとTextuteTextLineが扱う１文字分の画像データ
struct TxtureChar
{
	LPDIRECT3DTEXTURE9	m_txtr;	//テクスチャ画像
	int					m_wid;	//文字幅
};

//行揃え
enum TextAlign
{
	AlignLeft,
	AlignRight,
	AlignCenter
};

//TextGraphicが扱う1行分のテキスト
class TextuteTextLine : public std::vector<TxtureChar>
{
public:
	TextAlign	m_align;	//行揃え
	int			m_lineWidth;//１行の幅(ピクセル)

	TextuteTextLine();
	~TextuteTextLine();
};

//Graphic機能が扱うテキストデータ
class TextGraphic
{
	std::vector< TextuteTextLine* > m_lineAry;//行ごとにデータを持ちます。

	IRECT	m_rect;			//描画範囲
	int		m_fontsize;		//フォントサイズ
	DWORD	m_alphaColor;	//アルファ色( 描画するポリゴンのDiffuse色 )

public:

	TextGraphic();
	virtual ~TextGraphic();

	//テキスト画像を作成
	void Create( LPCTSTR txt, int left, int top, int right, int bottom, LOGFONT& logFont );
	
	//行そろえの指定(全行)
	void SetTextAlign( TextAlign align );

	//行そろえの指定(任意の複数行)
	void SetTextAlign(
		unsigned int lineIdx,	//開始位置
		unsigned int lineIdxTo,	//終了位置
		TextAlign align );

	//テキストのアルファ値(描画するポリゴンのDiffuse色)変更
	void SetTextAlpha(DWORD alpha);

	void Release();
	void Draw();
};