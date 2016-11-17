#pragma once
#include"DX11FbxLoader\DX11FbxLoader.h"


#ifdef _DEBUG
//デバッグビルド時
#ifdef _M_X64
//64bitビルド時
#ifdef _DLL
//ランタイムがDLLの時
#pragma comment(lib,"DX11FbxLoader-mdd_x64.lib")
#else
//ランタイムがDLLでない時
#pragma comment(lib,"DX11FbxLoader-md_x64.lib")
#endif

#else
//32bitビルド時
#ifdef _DLL
//ランタイムがDLLの時
#pragma comment(lib,"DX11FbxLoader-mdd_x86.lib")
#else
//ランタイムがDLLでない時
#pragma comment(lib,"DX11FbxLoader-md_x86.lib")
#endif


#endif

#else
//リリースビルド時
#ifdef _M_X64
//64bitビルド時

#ifdef _DLL
//ランタイムがDLLの時
#pragma comment(lib,"DX11FbxLoader-mtd_x64.lib")
#else
//ランタイムがDLLでない時
#pragma comment(lib,"DX11FbxLoader-mt_x64.lib")
#endif

#else
//32bitビルド時
#ifdef _DLL
//ランタイムがDLLの時
#pragma comment(lib,"DX11FbxLoader-mtd_x86.lib")
#else
//ランタイムがDLLでない時
#pragma comment(lib,"DX11FbxLoader-mt_x86.lib")
#endif


#endif

#endif
