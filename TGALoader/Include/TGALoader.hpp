#pragma once
#include"TgaLoader\TgaLoader.h"
#ifdef _DEBUG
//�f�o�b�O�r���h��
#ifdef _M_X64
//64bit�r���h��
#ifdef _DLL
//�����^�C����DLL�̎�
#pragma comment(lib,"TGALoader-mdd_x64.lib")
#else
//�����^�C����DLL�łȂ���
#pragma comment(lib,"TGALoader-md_x64.lib")
#endif

#else
//32bit�r���h��
#ifdef _DLL
//�����^�C����DLL�̎�
#pragma comment(lib,"TGALoader-mdd_x86.lib")
#else
//�����^�C����DLL�łȂ���
#pragma comment(lib,"TGALoader-md_x86.lib")
#endif


#endif

#else
//�����[�X�r���h��
#ifdef _M_X64
//64bit�r���h��

#ifdef _DLL
//�����^�C����DLL�̎�
#pragma comment(lib,"TGALoader-mtd_x64.lib")
#else
//�����^�C����DLL�łȂ���
#pragma comment(lib,"TGALoader-mt_x64.lib")
#endif

#else
//32bit�r���h��
#ifdef _DLL
//�����^�C����DLL�̎�
#pragma comment(lib,"TGALoader-mtd_x86.lib")
#else
//�����^�C����DLL�łȂ���
#pragma comment(lib,"TGALoader-mt_x86.lib")
#endif


#endif

#endif