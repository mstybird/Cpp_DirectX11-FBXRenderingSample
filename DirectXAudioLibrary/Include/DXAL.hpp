#pragma once
#include"DAL\MSAudio.hxx"
#include"DAL\SoundPlayer.h"
#ifdef _DEBUG
//�f�o�b�O�r���h��
#ifdef _M_X64
//64bit�r���h��
#ifdef _DLL
//�����^�C����DLL�̎�
#pragma comment(lib,"DirectXAudioLibrary-mdd_x64.lib")
#else
//�����^�C����DLL�łȂ���
#pragma comment(lib,"DirectXAudioLibrary-md_x64.lib")
#endif

#else
//32bit�r���h��
#ifdef _DLL
//�����^�C����DLL�̎�
#pragma comment(lib,"DirectXAudioLibrary-mdd_x86.lib")
#else
//�����^�C����DLL�łȂ���
#pragma comment(lib,"DirectXAudioLibrary-md_x86.lib")
#endif


#endif

#else
//�����[�X�r���h��
#ifdef _M_X64
//64bit�r���h��

#ifdef _DLL
//�����^�C����DLL�̎�
#pragma comment(lib,"DirectXAudioLibrary-mtd_x64.lib")
#else
//�����^�C����DLL�łȂ���
#pragma comment(lib,"DirectXAudioLibrary-mt_x64.lib")
#endif

#else
//32bit�r���h��
#ifdef _DLL
//�����^�C����DLL�̎�
#pragma comment(lib,"DirectXAudioLibrary-mtd_x86.lib")
#else
//�����^�C����DLL�łȂ���
#pragma comment(lib,"DirectXAudioLibrary-mt_x86.lib")
#endif


#endif

#endif