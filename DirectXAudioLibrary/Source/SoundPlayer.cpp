#include "SoundPlayer.h"
#include<XAudio2.h>
SoundPlayer::SoundPlayer(MSSourceVoice & aVoice, const XAUDIO2_BUFFER&aBuffer):
	mVoice{std::move(aVoice)}
{
	mBuffer = aBuffer;
}

void SoundPlayer::Play()
{
	if (mVoice.get() == nullptr)return;
	mVoice->Start();
}

void SoundPlayer::Stop(bool aReStart)
{
	if (mVoice.get() == nullptr)return;
	mVoice->Stop();
	if (aReStart) {
		mVoice->FlushSourceBuffers();
		if (FAILED(mVoice->SubmitSourceBuffer(&mBuffer))) {
			printf("Failed To Create Submit For SoundBuffer.\n");
		}
	}

}

void SoundPlayer::SetLoop(const bool aIsLoop)
{
	if(aIsLoop){
		mBuffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		if (FAILED(mVoice->SubmitSourceBuffer(&mBuffer))) {
			printf("Failed To Create Submit For SoundBuffer.\n");
		}
	}
	else {
		mBuffer.LoopCount = XAUDIO2_NO_LOOP_REGION;
		if (FAILED(mVoice->SubmitSourceBuffer(&mBuffer))) {
			printf("Failed To Create Submit For SoundBuffer.\n");
		}
	}
}

void SoundPlayer::SetVolume(float aVolume)
{
	if (mVoice.get() == nullptr)return;
	mVoice->SetVolume(aVolume, XAUDIO2_COMMIT_NOW);
}

float SoundPlayer::Volume() const
{
	if (mVoice.get() == nullptr)return 0.0f;
	float lVolume;
	mVoice->GetVolume(&lVolume);
	return lVolume;
}
