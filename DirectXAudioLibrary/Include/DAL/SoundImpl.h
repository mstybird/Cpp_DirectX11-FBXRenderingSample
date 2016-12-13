#pragma once
//#include"SoundBuffer.h"
#include"Deleter.hxx"
#include<unordered_map>
#include<memory>
#include<unordered_map>
class SoundBuffer;
struct XAUDIO2_BUFFER;
using MSSourceVoice = std::shared_ptr<IXAudio2SourceVoice/*, decltype(&VoiceDeleter)*/>;
using MSXAudio2 = std::shared_ptr<IXAudio2>;
class SoundDevice;
//�f�[�^�x�[�X
class SoundImpl {
public:
	SoundImpl(MSXAudio2&aXAudio);
private:
	friend class SoundDevice;
	//���O�o�^
	void PreLoad(const std::string&aFileName);
	//�o�^���ă{�C�X�쐬
	std::pair<XAUDIO2_BUFFER,MSSourceVoice>Load(const std::string & aFileName);
	std::unordered_map<std::string, SoundBuffer>mBuffer;

	MSXAudio2 mXAudio;//�O���ō쐬
};

