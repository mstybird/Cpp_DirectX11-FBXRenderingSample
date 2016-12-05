#pragma once
#define _WIN32_WINNT 0x0510
#include <windows.h>
#include <mmsystem.h>
#include <xaudio2.h>

#pragma comment(lib,"winmm.lib")
//#pragma comment(lib,"xmcore.lib")
//#pragma comment(lib,"xaudio2.lib")

#define _USE_VOICECALLBACK_

HANDLE g_hEvent = NULL;
HMMIO g_hmmio = NULL;
MMIOINFO g_mmioinfo;
MMCKINFO g_riffchunkinfo;
MMCKINFO g_datachunkinfo;
WAVEFORMATEX g_wfx;

IXAudio2 *g_lpXAudio = NULL;
IXAudio2MasteringVoice *g_lpMasterVoice = NULL;
IXAudio2SourceVoice *g_lpSourceVoice = NULL;

#define BUFFERQUEUE_MAX			4
#define BUFFERQUEUE_ALLOC		(BUFFERQUEUE_MAX + 1)

class CSoundCallback : public IXAudio2VoiceCallback {
public:
	CSoundCallback() { }
	~CSoundCallback() { }

	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) { }
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void) { }
	void STDMETHODCALLTYPE OnStreamEnd(void) { SetEvent(g_hEvent); }
	void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext) { SetEvent(g_hEvent); }
	void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext) { }
	void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext) { }
	void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error) { }
};

class MSSoundBGM {
public:
	void Initialize();
	bool LoadWave(const char*aFileName);

	HANDLE mHEvent = NULL;
	HMMIO mHmmio = NULL;
	MMIOINFO mMmioinfo;
	MMCKINFO mRiffchunkinfo;
	MMCKINFO mDatachunkinfo;
	WAVEFORMATEX mWfx;

	IXAudio2 *g_lpXAudio = NULL;
	IXAudio2MasteringVoice *g_lpMasterVoice = NULL;
	IXAudio2SourceVoice *g_lpSourceVoice = NULL;

	//サウンドコールバック
	CSoundCallback mCallBack;

};