#pragma once
#include"NcgLuaDatabase.h"
#ifdef _M_X64
#ifdef _DEBUG
#pragma comment(lib,"Lua/lib/lua5.3.3_x64d-md.lib")
#else
#pragma comment(lib,"Lua/lib/lua5.3.3_x64-mt.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib,"Lua/lib/lua5.3.3_x86-md.lib")
#else
#pragma comment(lib,"Lua/lib/lua5.3.3_x86-mt.lib")
#endif
#endif