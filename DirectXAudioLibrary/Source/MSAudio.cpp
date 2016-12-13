#include "MSAudio.hxx"
#include"SoundPlayer.h"
#include"SoundBuffer.h"
#include <xaudio2.h>
#include<cassert>
namespace {

	IXAudio2*CreateXAudio() {
		
#ifdef _DEBUG
		const uint32_t lFlag{ XAUDIO2_DEBUG_ENGINE };
#else
		const uint32_t lFlag{ 0 };
#endif
		
		IXAudio2 *lXAudio{ nullptr };
		//IXAudio2‚Ìì¬
		if (FAILED(XAudio2Create(&lXAudio, lFlag))) {
			printf("ERROR:%s\n", "Create Failed For IXAudio2\n");
		}
		return lXAudio;
	}

	IXAudio2MasteringVoice *CreateMasteringVoice(const MSXAudio2& aXAudio2) {
		assert(aXAudio2.get());
		IXAudio2MasteringVoice *lMasteringVoice{ nullptr };
		//IXAudio2MasteringVoice‚ðì¬
		if (FAILED(aXAudio2->CreateMasteringVoice(&lMasteringVoice))) {
			printf("ERROR:%s\n", "Create Failed For IXAudio2MasteringVoice");
		}
		return lMasteringVoice;
	}

}

SoundDevice::SoundDevice():
	mComIniter{},
	mXAudio{CreateXAudio(),IXAudio2Deleter},
	mMasteringVoice{CreateMasteringVoice(mXAudio),MasteringVoiceDeleter},
	mDatabase{mXAudio}
{
}

SoundDevice::~SoundDevice()
{

}

SoundPlayer SoundDevice::CreateSoundFromFile(const std::string & aFileName)
{
	auto lSound = mDatabase.Load(aFileName);
	
	return SoundPlayer(lSound.second,lSound.first);
}


