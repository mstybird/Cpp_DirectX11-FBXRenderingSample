#pragma once
#include"SpawnMapImporter\SpawnMapImporter.hxx"


#ifdef _DEBUG
//デバッグビルド時
#ifdef _M_X64
//64bitビルド時
#ifdef _DLL
//ランタイムがDLLの時
#pragma comment(lib,"SpawnMapImporter-mdd_x64.lib")
#else
//ランタイムがDLLでない時
#pragma comment(lib,"SpawnMapImporter-md_x64.lib")
#endif

#else
//32bitビルド時
#ifdef _DLL
//ランタイムがDLLの時
#pragma comment(lib,"SpawnMapImporter-mdd_x86.lib")
#else
//ランタイムがDLLでない時
#pragma comment(lib,"SpawnMapImporter-md_x86.lib")
#endif


#endif

#else
//リリースビルド時
#ifdef _M_X64
//64bitビルド時

#ifdef _DLL
//ランタイムがDLLの時
#pragma comment(lib,"SpawnMapImporter-mtd_x64.lib")
#else
//ランタイムがDLLでない時
#pragma comment(lib,"SpawnMapImporter-mt_x64.lib")
#endif

#else
//32bitビルド時
#ifdef _DLL
//ランタイムがDLLの時
#pragma comment(lib,"SpawnMapImporter-mtd_x86.lib")
#else
//ランタイムがDLLでない時
#pragma comment(lib,"SpawnMapImporter-mt_x86.lib")
#endif


#endif

#endif
