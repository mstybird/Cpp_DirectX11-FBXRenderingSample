#pragma once
#define _WIN32_DCOM
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#define MAX_WAV 100 //WAV�T�E���h�ő吔

//�}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(x) if(x){delete[] x; x=0;}

class MSAudio {
public:
	void InitializeSound();
	void PlayAudio(int aIndex);
	int LoadAudio(const char* aFileName);
	~MSAudio();
private:
	IXAudio2*mXAudio2;
	IXAudio2MasteringVoice*mMasteringVoice;
	IXAudio2SourceVoice* mSourceVoice;
	byte* mWavBuffer[MAX_WAV];
	DWORD mWavSize[MAX_WAV];
	int mSoundIndex;
};
