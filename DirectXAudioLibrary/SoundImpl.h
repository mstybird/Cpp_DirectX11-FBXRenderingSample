#pragma once
//#include"SoundBuffer.h"
#include"Deleter.hxx"
#include<unordered_map>
#include<memory>

class SoundBuffer;
using MSSourceVoice = std::unique_ptr<IXAudio2SourceVoice, decltype(&VoiceDeleter)>;
using MSXAudio2 = std::shared_ptr<IXAudio2>;
class SoundDevice;
//�f�[�^�x�[�X
class SoundImpl {
public:
	explicit SoundImpl(MSXAudio2&aXAudio);
private:
	friend class SoundDevice;
	//���O�o�^
	void PreLoad(const std::string&aFileName);
	//�o�^���ă{�C�X�쐬
	MSSourceVoice Load(const std::string & aFileName);
	std::unordered_map<std::string, SoundBuffer>mBuffer;

	MSXAudio2 mXAudio;//�O���ō쐬
};

