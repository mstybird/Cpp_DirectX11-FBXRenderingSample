#pragma once
#include"Deleter.hxx"
#include"ComIniter.h"
#include"SoundImpl.h"
#include<memory>
#pragma comment(lib,"winmm.lib")



struct IXAudio2;
struct IXAudio2MasteringVoice;
class SoundPlayer;
using MSXAudio2 = std::shared_ptr<IXAudio2>;
using MSMasterVoice = std::shared_ptr<IXAudio2MasteringVoice/*, decltype(&MasteringVoiceDeleter)*/>;
class SoundDevice {
public:
	SoundDevice();
	~SoundDevice();

	SoundPlayer CreateSoundFromFile(const std::string&aFileName);

	ComIniter mComIniter;
	MSXAudio2 mXAudio;
	MSMasterVoice mMasteringVoice;

	SoundImpl mDatabase;//サウンドデータベース


};

