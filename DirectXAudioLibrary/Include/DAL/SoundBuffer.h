#pragma once
#include<vector>
#include<string>
#include<XAudio2.h>

//�T�E���h�o�b�t�@���̂���
class SoundBuffer {
public:
	SoundBuffer();
	bool LoadFile(const std::string&aFileName);
public:
	std::vector<char> mBuffer;
	WAVEFORMATEX mFormat;
};