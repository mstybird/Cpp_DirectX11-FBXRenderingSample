#pragma once
#include"AIMapImport\AIMapImport.hxx"
#ifdef _DEBUG
#ifdef _DLL
#pragma comment(lib,"AIMapImporter-mdd.lib")
#else
#pragma comment(lib,"AIMapImporter-md.lib")
#endif
#else
#ifdef _DLL
#pragma comment(lib,"AIMapImporter-mtd.lib")
#else
#pragma comment(lib,"AIMapImporter-mt.lib")
#endif

#endif