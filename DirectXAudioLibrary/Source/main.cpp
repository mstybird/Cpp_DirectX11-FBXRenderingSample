#pragma once
#define _WIN32_WINNT 0x0510
#include"MSAudio.hxx"
#include"SoundBuffer.h"
#include"SoundPlayer.h"
#include<Windows.h>
#include<iostream>
#include<vector>
#include<time.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"libvorbis_mtd.lib")
#pragma comment(lib,"libvorbisfile_mtd.lib")
#pragma comment(lib,"libogg_mtd.lib")

//void InitializeXAudio() {
//	//XAudio2の初期化
//	HRESULT lResult;
//	UINT32 lFlags{ 0 };
//	XAudio2Create(&gXAudio, lFlags);
//
//	gXAudio->CreateMasteringVoice(&gMasteringVoice);
//}
//
//void CreateBuffer() {
//	WAVEFORMATEX lFormat{ 0 };
//	lFormat.wFormatTag = WAVE_FORMAT_PCM;
//	//チャンネル数
//	lFormat.nChannels = 1;
//	//1サンプルあたりのビット数
//	lFormat.wBitsPerSample = 16;
//	//サンプリングレート
//	lFormat.nSamplesPerSec = 44100;
//
//	lFormat.nBlockAlign = lFormat.wBitsPerSample / 8 * lFormat.nChannels;
//
//	lFormat.nAvgBytesPerSec = lFormat.nSamplesPerSec*lFormat.nBlockAlign;
//
//
//	//1秒分のバッファ
//	std::vector<short>lBufferData(lFormat.nAvgBytesPerSec * 1);
//	for (auto i = 0; i < lBufferData.size(); ++i) {
//		float lLength = lFormat.nSamplesPerSec / 440.0f;
//		lBufferData[i] = (short)(32767 * sinf(i*3.141592f / (lLength / 2)));
//	}
//
//	gXAudio->CreateSourceVoice(&gSourceVoice, &lFormat);
//
//	//SourceVoiceにデータ送信
//	XAUDIO2_BUFFER lBuffer{ 0 };
//	lBuffer.AudioBytes = lBufferData.size();
//	lBuffer.pAudioData = (byte*)(lBufferData.data());
//	lBuffer.Flags = XAUDIO2_END_OF_STREAM;
//	gSourceVoice->SubmitSourceBuffer(&lBuffer);
//
//	//再生
//	gSourceVoice->Start();
//	MessageBox(nullptr, "End", "", 0);
//	//SourceVoiceの破棄
//	gSourceVoice->Stop();
//	gSourceVoice->DestroyVoice();
//
//}
//
//
//
//void CleanUp() {
//	//CleanUp XAudio2
//
//	if (gMasteringVoice != nullptr) {
//		gMasteringVoice->DestroyVoice();
//		gMasteringVoice = nullptr;
//	}
//	if (gXAudio != nullptr) {
//		gXAudio->Release();
//		gXAudio = nullptr;
//	}
//
//}

std::unique_ptr<SoundDevice> sd;


int main() {
	SoundDevice sd;

	SoundPlayer sp = sd.CreateSoundFromFile("test.wav");
	sp.Play();
	clock_t start = 0;
	while (1)
	{
		start = clock();
		while (start + 5000 > clock());
		sp.Stop(true);
		sp.Play();
		start = clock();
		while (start + 5000 > clock());

	}
	//COMの初期化
	//MSWaveLoader m;
	//WAVEFORMATEX format;
	//WAVEHDR hdr;
	//m.LoadWaveFile("test.wav", &format, &hdr);

	////InitializeXAudio();

	////CreateBuffer();

	////CleanUp();
	
	return 0;
}