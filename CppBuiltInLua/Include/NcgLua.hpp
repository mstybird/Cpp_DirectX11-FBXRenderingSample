#pragma once
#include"NcgLua\NcgLuaDatabase.hxx"


#ifdef _DEBUG
//�f�o�b�O�r���h��
#ifdef _M_X64
//64bit�r���h��
#ifdef _DLL
//�����^�C����DLL�̎�
#pragma comment(lib,"CppBuiltInLua-mdd_x64.lib")
#else
//�����^�C����DLL�łȂ���
#pragma comment(lib,"CppBuiltInLua-md_x64.lib")
#endif

#else
//32bit�r���h��
#ifdef _DLL
//�����^�C����DLL�̎�
#pragma comment(lib,"CppBuiltInLua-mdd_x86.lib")
#else
//�����^�C����DLL�łȂ���
#pragma comment(lib,"CppBuiltInLua-md_x86.lib")
#endif


#endif

#else
//�����[�X�r���h��
#ifdef _M_X64
//64bit�r���h��

#ifdef _DLL
//�����^�C����DLL�̎�
#pragma comment(lib,"CppBuiltInLua-mtd_x64.lib")
#else
//�����^�C����DLL�łȂ���
#pragma comment(lib,"CppBuiltInLua-mt_x64.lib")
#endif

#else
//32bit�r���h��
#ifdef _DLL
//�����^�C����DLL�̎�
#pragma comment(lib,"CppBuiltInLua-mtd_x86.lib")
#else
//�����^�C����DLL�łȂ���
#pragma comment(lib,"CppBuiltInLua-mt_x86.lib")
#endif


#endif

#endif