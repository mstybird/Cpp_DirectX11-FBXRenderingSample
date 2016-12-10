#include "SoundPlayer.h"
#include<XAudio2.h>
SoundPlayer::SoundPlayer(MSSourceVoice & aVoice):
	mVoice{std::move(aVoice)}
{

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
