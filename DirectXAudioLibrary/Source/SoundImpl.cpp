#include "SoundImpl.h"
#include"SoundBuffer.h"
#include<XAudio2.h>
namespace {
	IXAudio2SourceVoice*CreateSourceVoice(MSXAudio2&aXAudio ,const WAVEFORMATEX &aFormat) {
		IXAudio2SourceVoice *lVoice{ nullptr };
		//Create For IXAudio2SourceVoice
		if (FAILED(aXAudio->CreateSourceVoice(&lVoice, &aFormat))) {
			printf("Failed To Create Sound Buffer.\n");
		}
		return lVoice;
	}
}

SoundImpl::SoundImpl(MSXAudio2 & aXAudio)
{
	mXAudio = aXAudio;

}

void SoundImpl::PreLoad(const std::string & aFileName)
{
	if (mBuffer.count(aFileName) != 0) {
		printf("Registered %s\n", aFileName.c_str());
		return;
	}
	mBuffer[aFileName] = SoundBuffer();
	mBuffer[aFileName].LoadFile(aFileName);

}

std::pair<XAUDIO2_BUFFER, MSSourceVoice> SoundImpl::Load(const std::string & aFileName)
{
	PreLoad(aFileName);

	MSSourceVoice lVoice{ CreateSourceVoice(mXAudio,mBuffer[aFileName].mFormat), VoiceDeleter };

	const XAUDIO2_BUFFER submit = {
		XAUDIO2_END_OF_STREAM,
		mBuffer[aFileName].mBuffer.size(),
		reinterpret_cast<const BYTE *>(&mBuffer[aFileName].mBuffer.front()),
		0,
		0,
		XAUDIO2_NO_LOOP_REGION,
		0,
		0,
		nullptr
	};

	

	//–Â‚ç‚·”gŒ`‚Ì“o˜^
	if (FAILED(lVoice->SubmitSourceBuffer(&submit))) {
		printf("Failed To Create Submit For SoundBuffer.\n");
	}

	return{ submit,std::move(lVoice) };
}

