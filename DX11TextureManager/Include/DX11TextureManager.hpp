#pragma once
#include"DX11TextureManager\DX11TextureManager.h"
#include"DX11TextureManager\DX11Texture.h"

#ifdef _DEBUG
//デバッグビルド時
#ifdef _M_X64
//64bitビルド時
#ifdef _DLL
//ランタイムがDLLの時
#pragma comment(lib,"DX11TextureManager-mdd_x64.lib")
#else
//ランタイムがDLLでない時
#pragma comment(lib,"DX11TextureManager-md_x64.lib")
#endif

#else
//32bitビルド時
#ifdef _DLL
//ランタイムがDLLの時
#pragma comment(lib,"DX11TextureManager-mdd_x86.lib")
#else
//ランタイムがDLLでない時
#pragma comment(lib,"DX11TextureManager-md_x86.lib")
#endif


#endif

#else
//リリースビルド時
#ifdef _M_X64
//64bitビルド時

#ifdef _DLL
//ランタイムがDLLの時
#pragma comment(lib,"DX11TextureManager-mtd_x64.lib")
#else
//ランタイムがDLLでない時
#pragma comment(lib,"DX11TextureManager-mt_x64.lib")
#endif

#else
//32bitビルド時
#ifdef _DLL
//ランタイムがDLLの時
#pragma comment(lib,"DX11TextureManager-mtd_x86.lib")
#else
//ランタイムがDLLでない時
#pragma comment(lib,"DX11TextureManager-mt_x86.lib")
#endif


#endif

#endif
