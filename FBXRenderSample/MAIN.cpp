#include "MAIN.h"


/*
	メモ：
	次の作業
	GetNodeのアニメーション反映
*/



//グローバル変数
MAIN* g_pMain=NULL;
//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
//
//
//アプリケーションのエントリー関数 
INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,INT)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#ifdef _DEBUG
	AllocConsole();
	FILE *fp;
	AllocConsole();
	freopen_s(&fp, "CON", "w", stdout);
#endif
	g_pMain=new MAIN;
	if(g_pMain != NULL)
	{
		//FBX初期化
		if(SUCCEEDED(g_pMain->InitWindow(hInstance,0,0,WINDOW_WIDTH,
			WINDOW_HEIGHT,APP_NAME)))
		{
			if(SUCCEEDED(g_pMain->InitD3D()))
			{
				g_pMain->Loop();
			}
		}

		//アプリ終了
		delete g_pMain;
	}
#ifdef _DEBUG
	fclose(fp);
	FreeConsole();
#endif
	int a = 0;

	return 0;
}
//
//
//OSから見たウィンドウプロシージャー（実際の処理はMAINクラスのプロシージャーで処理）
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd,uMsg,wParam,lParam);
}
//
//
//クラスコンストラクター
MAIN::MAIN()
{
	//ZeroMemory(this,sizeof(MAIN));
}
//
//
//クラスデストラクター
MAIN::~MAIN()
{
	fbx.FbxDestroy();
	DestroyD3D();
}
//
//
//ウィンドウ作成
HRESULT MAIN::InitWindow(HINSTANCE hInstance,
		INT iX,INT iY,INT iWidth,INT iHeight,LPSTR WindowName)
{
	// ウィンドウの定義
	WNDCLASSEX  wc;
	ZeroMemory(&wc,sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	RegisterClassEx(&wc);
	//ウィンドウの作成
	m_hWnd=CreateWindow(WindowName,WindowName,WS_OVERLAPPEDWINDOW,
	 0,0,iWidth,iHeight,0,0,hInstance,0);
	if(!m_hWnd)
	{
	 return E_FAIL;
	}
	//ウインドウの表示
	ShowWindow(m_hWnd,SW_SHOW);
	UpdateWindow(m_hWnd) ;

	return S_OK;
}
//
//
//ウィンドウプロシージャー
LRESULT MAIN::MsgProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_KEYDOWN:
		switch((char)wParam)
		{
			case VK_ESCAPE://ESCキーで修了
			PostQuitMessage(0);
			break;
		}
		break;
		case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc (hWnd, iMsg, wParam, lParam);
}
//
//
//メッセージループとアプリケーション処理の入り口
void MAIN::Loop()
{
	// メッセージループ
	MSG msg={0};
	ZeroMemory(&msg,sizeof(msg));
	while(msg.message!=WM_QUIT)
	{
		if( PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//FBXループ処理
			fbx.FbxProc();
			//アプリケーションの処理はここから飛ぶ。
			App();
		}
	}
}
//
//
//アプリケーション処理。アプリのメイン関数。
void MAIN::App()
{
	Render();
}
//
//
//Direct3D初期化
HRESULT MAIN::InitD3D()
{
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width=WINDOW_WIDTH;
	sd.BufferDesc.Height=WINDOW_HEIGHT;
	sd.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator=60;
	sd.BufferDesc.RefreshRate.Denominator=1;
	sd.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow=m_hWnd;
	sd.SampleDesc.Count=1;
	sd.SampleDesc.Quality=0;
	sd.Windowed=TRUE;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;
	
	if( FAILED( D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,
		0,&pFeatureLevels,1,D3D11_SDK_VERSION,&sd,&m_pSwapChain,&m_pDevice,
		pFeatureLevel,&m_pDeviceContext )))
	{
		return FALSE;
	}
	//各種テクスチャーと、それに付帯する各種ビューを作成

	//バックバッファーテクスチャーを取得（既にあるので作成ではない）
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ),(LPVOID*)&pBackBuffer_Tex);
	//そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
	m_pDevice->CreateRenderTargetView( pBackBuffer_Tex, NULL, &m_pBackBuffer_TexRTV );
	SAFE_RELEASE(pBackBuffer_Tex);	

	//デプスステンシルビュー用のテクスチャーを作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D( &descDepth, NULL, &m_pBackBuffer_DSTex );
	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	m_pDevice->CreateDepthStencilView( m_pBackBuffer_DSTex, NULL, &m_pBackBuffer_DSTexDSV);

	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV,m_pBackBuffer_DSTexDSV);
	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports( 1, &vp );
	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc,sizeof(rdc));
	rdc.CullMode=D3D11_CULL_NONE;
	rdc.FillMode=D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;
	
	m_pDevice->CreateRasterizerState(&rdc,&m_pRasterizerState);
	m_pDeviceContext->RSSetState(m_pRasterizerState);	
	//シェーダー初期化
	if(FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//ポリゴン作成
	if(FAILED(InitPolygon()))
	{
		return E_FAIL;
	}

	return S_OK;
}
//
//
//シェーダーを作成　頂点レイアウトを定義
HRESULT MAIN::InitShader()
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader=NULL;
	ID3DBlob *pErrors=NULL;
	//ブロブからバーテックスシェーダー作成
	if(FAILED(D3DX11CompileFromFile("Simple.hlsl",NULL,NULL,"VS","vs_5_0",0,0,NULL,&pCompiledShader,&pErrors,NULL)))
	{
		MessageBox(0,"hlsl読み込み失敗",NULL,MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if(FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,"バーテックスシェーダー作成失敗",NULL,MB_OK);
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout)/sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if( FAILED( m_pDevice->CreateInputLayout( layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout ) ) )
	{
		return FALSE;
	}
	//ブロブからピクセルシェーダー作成
	if(FAILED(D3DX11CompileFromFile("Simple.hlsl",NULL,NULL,"PS","ps_5_0",0,0,NULL,&pCompiledShader,&pErrors,NULL)))
	{
		MessageBox(0,"hlsl読み込み失敗",NULL,MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if(FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,"ピクセルシェーダー作成失敗",NULL,MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);		
	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth= sizeof( SIMPLESHADER_CONSTANT_BUFFER );
	cb.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags	=0;
	cb.StructureByteStride=0;
	cb.Usage=D3D11_USAGE_DYNAMIC;

	if( FAILED(m_pDevice->CreateBuffer( &cb,NULL,&m_pConstantBuffer)))
	{
		return E_FAIL;
	}
	return S_OK;
}
//
//
//バーテックスバッファー作成



HRESULT MAIN::InitPolygon()
{
	D3DXVECTOR3 v0(1,0,0);
	D3DXVECTOR3 v1(0,2,0);
	D3DXVECTOR3 v2(1,1,1);
	D3DXVECTOR3 vv,vv1, vv2;

	vv1 = v1 - v0;
	vv2 = v2 - v1;
	D3DXVec3Cross(&vv, &vv1, &vv2);
	D3DXVec3Normalize(&vv, &vv);


	FbxVector4 fv0(1, 0, 0);
	FbxVector4 fv1(0, 2, 0);
	FbxVector4 fv2(1, 1, 1);
	FbxVector4 fvv, fvv1, fvv2;
	fvv1 = fv1 - fv0;
	fvv2 = fv2 - fv1;
	fvv = fvv1.CrossProduct(fvv2);
	fvv.Normalize();

	auto sv1 = vv;
	auto sv2 = fvv;

	fbx.FbxInit("res/humanoid2.fbx");
	vert = fbx.GetGeometryData();


	//上の頂点でバーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex)*vert->at(0).at(0)->PosLength;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vert->at(0).at(0)->Data.data();
	if( FAILED( m_pDevice->CreateBuffer( &bd, &InitData, &m_pVertexBuffer ) ) )
	{
		return E_FAIL;
	}


	//インデックスバッファーを作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * vert->at(0).at(0)->IndexLength;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = vert->at(0).at(0)->Index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer)))
		return E_FAIL;


	return S_OK;
}
//
//
//シーンを画面にレンダリング
void MAIN::Render()
{

	auto start = timeGetTime();
	vert = fbx.GetGeometryData();
	auto end = timeGetTime();
	FBXSDK_printf("Time:%2d\n", (int)(end - start));

	

	//上の頂点でバーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex)*vert->at(0).at(0)->PosLength;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vert->at(0).at(0)->Data.data();;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
	//	return E_FAIL;
	}






	//画面クリア（実際は単色で画面を塗りつぶす処理）
	float ClearColor[4] = {0,0,1,1};// クリア色作成　RGBAの順
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer_TexRTV,ClearColor);//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pBackBuffer_DSTexDSV,D3D11_CLEAR_DEPTH,1.0f,0);//深度バッファクリア

	D3DXMATRIX mWorld,mRotate;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	//ワールドトランスフォーム（絶対座標変換）
	D3DXMatrixScaling(&mWorld, 1,1,1);
	D3DXMatrixRotationY( &mRotate, /*timeGetTime()/1000.0f*/0 );//単純にyaw回転させる
	// ビュートランスフォーム（視点座標変換）
	D3DXVECTOR3 vEyePt( 0.0f, 20.0f,-120.0f ); //カメラ（視点）位置
	D3DXVECTOR3 vLookatPt( 0.0f, 20.0f, 0.0f );//注視位置
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );//上方位置
	D3DXMatrixLookAtLH( &mView, &vEyePt, &vLookatPt, &vUpVec );
	// プロジェクショントランスフォーム（射影変換）
	D3DXMatrixPerspectiveFovLH( &mProj, D3DX_PI/4, (FLOAT)WINDOW_WIDTH/(FLOAT)WINDOW_HEIGHT, 0.1f, 2000.0f );
	//使用するシェーダーの登録
	m_pDeviceContext->VSSetShader(m_pVertexShader,NULL,0);
	m_pDeviceContext->PSSetShader(m_pPixelShader,NULL,0);
	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if( SUCCEEDED( m_pDeviceContext->Map( m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m=(mRotate*mWorld)*mView*mProj;
		D3DXMatrixTranspose( &m, &m );
		cb.mWVP=m;
		cb.mW = mRotate*mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		cb.LightDir = D3DXVECTOR4(1, 0, -1, 0);
		cb.Diffuse = D3DXVECTOR4(1, 0, 0, 0);

		memcpy_s( pData.pData, pData.RowPitch, (void*)( &cb), sizeof( cb ) );
		m_pDeviceContext->Unmap( m_pConstantBuffer, 0 );
	}

	//このコンスタントバッファーを、どのシェーダーで使うかを指定
	m_pDeviceContext->VSSetConstantBuffers(0,1,&m_pConstantBuffer );//バーテックスバッファーで使う
	m_pDeviceContext->PSSetConstantBuffers(0,1,&m_pConstantBuffer );//ピクセルシェーダーでの使う
	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );


	stride = sizeof(int);
	offset = 0;
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout( m_pVertexLayout );
	//プリミティブ・トポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	//プリミティブをレンダリング
	//m_pDeviceContext->Draw(vert[0][0].Pos.size(),3);
	m_pDeviceContext->DrawIndexed(vert->at(0).at(0)->IndexLength, 0, 0);
	m_pSwapChain->Present(1,0);//画面更新（バックバッファをフロントバッファに）	
}

//
//
//全てのインターフェイスをリリース
void MAIN::DestroyD3D()
{
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

MaterialCache::MaterialCache():
	Shinness(0)
{

}

MaterialCache::~MaterialCache()
{
}

//取得済みのマテリアルから色を取得
bool MaterialCache::Initialize(const FbxSurfaceMaterial * pMaterial)
{
	//エミッシブの取得
	const auto lEmissive = GetMaterialProperty(pMaterial, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, Emissive.TextureName);
	Emissive.Color[0] = static_cast<FbxFloat>(lEmissive[0]);
	Emissive.Color[1] = static_cast<FbxFloat>(lEmissive[1]);
	Emissive.Color[2] = static_cast<FbxFloat>(lEmissive[2]);


	const FbxDouble3 lAmbient = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, Ambient.TextureName);
	Ambient.Color[0] = static_cast<FbxFloat>(lAmbient[0]);
	Ambient.Color[1] = static_cast<FbxFloat>(lAmbient[1]);
	Ambient.Color[2] = static_cast<FbxFloat>(lAmbient[2]);

	const FbxDouble3 lDiffuse = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, Diffuse.TextureName);
	Diffuse.Color[0] = static_cast<FbxFloat>(lDiffuse[0]);
	Diffuse.Color[1] = static_cast<FbxFloat>(lDiffuse[1]);
	Diffuse.Color[2] = static_cast<FbxFloat>(lDiffuse[2]);

	const FbxDouble3 lSpecular = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, Specular.TextureName);
	Specular.Color[0] = static_cast<FbxFloat>(lSpecular[0]);
	Specular.Color[1] = static_cast<FbxFloat>(lSpecular[1]);
	Specular.Color[2] = static_cast<FbxFloat>(lSpecular[2]);

	auto lShininessProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	if (lShininessProperty.IsValid()) {
		auto lShininess = lShininessProperty.Get<FbxDouble>();
		Shinness = static_cast<FbxFloat>(lShininess);
	}

	return true;
}

FbxDouble3 MaterialCache::GetMaterialProperty(const FbxSurfaceMaterial * pMaterial, FbxString pPropertyName, FbxString pFactorPropertyName, FbxUInt & pTextureName)
{
	FbxDouble3 lResult(0, 0, 0);
	const auto lProperty = pMaterial->FindProperty(pPropertyName);
	const auto lFactorProperty = pMaterial->FindProperty(pFactorPropertyName);

	if (lProperty.IsValid()&lFactorProperty.IsValid()) {
		lResult = lProperty.Get<FbxDouble3>();
		auto lFactor = lFactorProperty.Get<FbxDouble>();
		if (lFactor != 1) {
			lResult[0] *= lFactor;
			lResult[1] *= lFactor;
			lResult[2] *= lFactor;

		}
	}

	if (lProperty.IsValid()) {
		const auto lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
		if (lTextureCount) {
			const auto lTexture = lProperty.GetSrcObject<FbxFileTexture>();
			if (lTexture&&lTexture->GetUserDataPtr()) {

				auto lTextureName = *(static_cast<FbxString*>(lTexture->GetUserDataPtr()));
				pTextureName = *(static_cast<FbxUInt*>(lTexture->GetUserDataPtr()));
			}
		}
	}

	return lResult;
}


