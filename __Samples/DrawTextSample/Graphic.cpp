//-----------------------------------------------------
//	DirectX テキスト描画サンプル
//	Direct3Dの描画関連
//	2010.03.04 TMO
//-----------------------------------------------------

#include "stdafx.h"
#include "Graphic.h"

//「D3DFVF_TEX1」の1はテクスチャの枚数
#define D3DFVF_2DVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE )
typedef struct _D3DVERTEX_
{   D3DVECTOR	pos;		//頂点の位置
	float		rhw;		//1をいれとく
	D3DCOLOR    color;		//diffuse	ポリゴンの色。画面の部分/全体フェードとかテクスチャ画像のアルファ値を制御するのに使える
	FLOAT       tu;			//テクスチャの座標
	FLOAT       tv;			//テクスチャの座標

} D3DVERTEX;

CGraphic*				glbl_graphic = NULL;

LPDIRECT3D9				g_pD3D = NULL;			//D3Dインターフェース D3Dデバイスを作るのに必要
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;	//D3Dデバイス 描画など広範囲で必要になる。
IDirect3DVertexBuffer9* g_pVB = NULL;			//ポリゴンのバッファ

//他のコードから作成されたD3Dデバイスを取得するためのもの
extern LPDIRECT3DDEVICE9 GetDevice()
{
	return g_pD3DDevice;
}

//CGraphicの作成
CGraphic* CreateGraphic( HWND hWnd )
{
	glbl_graphic = new CGraphic();
	glbl_graphic->Create( hWnd );
	return glbl_graphic;
}

//CGraphicの取得
CGraphic* GetGraphic() { return glbl_graphic; }

//CGraphicの解放
void ReleaseGraphic()
{
	if ( glbl_graphic ) {
		glbl_graphic->Destroy();
		delete glbl_graphic;
		glbl_graphic = NULL;
	}
}


CGraphic::CGraphic()
{
	if ( g_pD3D != NULL ) {
		//CGraphicは２個作れないことにしている。
		_ASSERT(0);
	}

	m_backColor = 0xFFFFFFFF;

	m_currDisplayMode = DisplayMode::WindowMode;

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	
	g_pD3DDevice   = NULL;
}
CGraphic::~CGraphic()
{
	Destroy();
}

//CGraphicの作成
void CGraphic::Destroy()
{
	//D3Dデバイスを解放する
	if( g_pD3DDevice )
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//D3Dインターフェースを解放する
	if( g_pD3D )
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//CGraphicの作成( Direct3Dの初期化処理 )
bool CGraphic::Create(HWND hWnd)
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if ( g_pD3D == NULL ) {
		return false;
	}
	
	//D3DPRESENT_PARAMETERS構造体初期化のためディスプレイモードを取得する
	D3DDISPLAYMODE dmode;
	ZeroMemory( &dmode, sizeof( dmode ) );
	g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &dmode );
	
	//デバイスの状態を指定する
	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );

	switch ( m_currDisplayMode ) {
		case DisplayMode::FullScreen:
			//フルスクリーン
			d3dpp.Windowed         = FALSE;
			d3dpp.BackBufferWidth  = 640;
			d3dpp.BackBufferHeight = 480;
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
			d3dpp.BackBufferCount  = 1;
			d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
			d3dpp.FullScreen_RefreshRateInHz = 60;
			break;
		case DisplayMode::WindowMode:
			//ウィンドウモード
			d3dpp.Windowed			= TRUE;
			d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat	= dmode.Format;
			d3dpp.BackBufferCount	= 1;
			break;
	}

	//ディスプレイアダプタを表すためのデバイスを作成する
	//描画処理方法と頂点処理方法はハードウェア処理が未対応であると失敗するので、処理方法の組み合わせを変えて再度作成を試みる
	HRESULT		hr;
	D3DDEVTYPE	devtype;
	DWORD		behaviorFlags;
	for ( int c = 0; c < 4; c++ ) {
		switch ( c ) {
            case 0:
				//ハードウェアで描画処理を行う
				devtype			= D3DDEVTYPE_HAL;						
				//頂点処理をハードウェアで行う
				behaviorFlags	= D3DCREATE_HARDWARE_VERTEXPROCESSING;
				break;
			case 1:
				//ハードウェアで描画処理を行う
				devtype			= D3DDEVTYPE_HAL;
				//頂点処理をソフトウェアで行う
				behaviorFlags	= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
				break;
			case 2:
				//ソフトウェアで描画処理を行う
				devtype			= D3DDEVTYPE_REF;
				//頂点処理をハードウェアで行う
				behaviorFlags	= D3DCREATE_HARDWARE_VERTEXPROCESSING;
				break;
			case 3:
				//ソフトウェアで描画処理を行う
				devtype			= D3DDEVTYPE_REF;						
				//頂点処理をソフトウェアで行う
				behaviorFlags	= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
				break;
		}
		
		hr = g_pD3D->CreateDevice(
						D3DADAPTER_DEFAULT,
						devtype,
						hWnd,
						behaviorFlags,
						&d3dpp,
						&g_pD3DDevice );
		
		
		if ( FAILED(hr) ) {
			continue;
		}
		break;
	}

	if ( FAILED(hr) ) {
		//ソフトウェア処理にも対応していない
		return false;
	}

	//アルファブレンドの設定
	g_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	//有効化だけでは思い通りに透過しないはずなので、ブレンディング係数の設定も行う。
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	 
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); 

	//テクスチャのアルファに、ポリゴン色をアルファとして加味する設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	//ポリゴンの作成
	g_pD3DDevice->CreateVertexBuffer(sizeof(D3DVERTEX)*4, 0, D3DFVF_2DVERTEX, D3DPOOL_MANAGED, &g_pVB, NULL );

	return true;
}

//描画前に呼ぶ
void CGraphic::ClearAndBegin()
{
	//画面クリア
	g_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, m_backColor, 1.0f, 0 );

	//デバイスに描画開始を知らせる
	HRESULT hr;
	hr = g_pD3DDevice->BeginScene();
}

//描画後に呼ぶ
void CGraphic::EndAndPresent()
{
	//デバイスに描画終了を知らせる
	g_pD3DDevice->EndScene();
	
	//画面をバックバッファと入れ替える。
	g_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

//色の入った矩形を描画。半透明できます。部分/全体のフェードに使える
void CGraphic::DrawColorRect( float left, float top, float right, float bottom, DWORD color )
{
	D3DVERTEX *d3dv;
	g_pVB->Lock(0 , 0 , (void**)&d3dv , 0);
	d3dv[0].pos.x = left;
	d3dv[0].pos.y = top;
	d3dv[0].pos.z = 0;
	d3dv[0].rhw = 1;
	d3dv[0].color = color;
	d3dv[0].tu = 0.0f;
	d3dv[0].tv = 0.0f;

	d3dv[1] = d3dv[0];
	d3dv[1].pos.x = right;
	d3dv[1].pos.y = top;
	d3dv[1].tu = 1.0f;
	d3dv[1].tv = 0.0f;

	d3dv[2] = d3dv[0];
	d3dv[2].pos.x = left;
	d3dv[2].pos.y = bottom;
	d3dv[2].tu = 0.0f;
	d3dv[2].tv = 1.0f;

	d3dv[3] = d3dv[0];
	d3dv[3].pos.x = right;
	d3dv[3].pos.y = bottom;
	d3dv[3].tu = 1.0f;
	d3dv[3].tv = 1.0f;

	g_pVB->Unlock();
	
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetVertexShader(NULL);
	g_pD3DDevice->SetFVF( D3DFVF_2DVERTEX );
	g_pD3DDevice->SetStreamSource( 0, g_pVB, 0, sizeof(D3DVERTEX) );
	g_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

}

//描画
void CGraphic::Draw( float left, float top, float right, float bottom, DWORD alphaColor )
{
	//指定位置そのままで描くとポリゴンの境界でわずかなずれが発生します。浮動小数点を使用しているためだとか・・・。
	//頂点座標をずらすことできれいに描ける。
	left	-= 0.5f;
	top		-= 0.5f;
	right	-= 0.5f;
	bottom	-= 0.5f;

	D3DVERTEX *d3dv;
	g_pVB->Lock(0 , 0 , (void**)&d3dv , 0);
	d3dv[0].pos.x = left;
	d3dv[0].pos.y = top;
	d3dv[0].pos.z = 0;
	d3dv[0].rhw = 1;
	d3dv[0].color = alphaColor;
	d3dv[0].tu = 0.0f;
	d3dv[0].tv = 0.0f;

	d3dv[1] = d3dv[0];
	d3dv[1].pos.x = right;
	d3dv[1].pos.y = top;
	d3dv[1].tu = 1.0f;
	d3dv[1].tv = 0.0f;

	d3dv[2] = d3dv[0];
	d3dv[2].pos.x = left;
	d3dv[2].pos.y = bottom;
	d3dv[2].tu = 0.0f;
	d3dv[2].tv = 1.0f;

	d3dv[3] = d3dv[0];
	d3dv[3].pos.x = right;
	d3dv[3].pos.y = bottom;
	d3dv[3].tu = 1.0f;
	d3dv[3].tv = 1.0f;

	g_pVB->Unlock();
	
	g_pD3DDevice->SetVertexShader(NULL);
	g_pD3DDevice->SetFVF( D3DFVF_2DVERTEX );
	g_pD3DDevice->SetStreamSource( 0, g_pVB, 0, sizeof(D3DVERTEX) );
	g_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
}

void CGraphic::Draw( float left, float top, float right, float bottom, CTexture* texture, DWORD alphaColor )
{
	texture->SetCurrentUse();
	Draw( left, top, right, bottom, alphaColor );
}