#pragma once
#include"ComIniter.h"
#include<XAudio2.h>
#include<string>
#include<vector>


//class SoundDevice {
//public:
//	SoundDevice();
//	~SoundDevice();
//
//	SoundPlayer CreateSoundFromFile(const std::string&aFileName);
//
//	ComIniter mComIniter;
//	MSXAudio2 mXAudio;
//	MSMasterVoice mMasteringVoice;
//
//	SoundImpl mDatabase;//�T�E���h�f�[�^�x�[�X
//
//
//};


class WaveStreamBuffer {
public:
	WaveStreamBuffer();
	void Next() { ++mCurrentBuffer %= BufferCount(); }
	uint8_t* Get() { return mBuffer[mCurrentBuffer]; }
	size_t BufferCount()const { return BUFFER_COUNT; }
	size_t BufferSize()const { return BUFFER_SIZE; }

private:
	static const size_t BUFFER_COUNT = 3;
	static const size_t BUFFER_SIZE = 1764;
	uint8_t mBuffer[BUFFER_COUNT][BUFFER_SIZE];
	uint8_t mCurrentBuffer;
};


class WaveDecoder {
public:
	//�t�H�[�}�b�g��͂܂�
	WaveDecoder(const std::string&aFileName);
	//�o�b�t�@���\�[�X
	std::vector<char> mBuffer;
	WAVEFORMATEX mFormat;

};



