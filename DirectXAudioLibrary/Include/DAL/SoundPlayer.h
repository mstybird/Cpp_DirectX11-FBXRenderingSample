#pragma once
//#include"SoundFwd.h"
#include"SoundImpl.h"
#include<XAudio2.h>
class SoundImpl;
class SoundPlayer {
public:
	SoundPlayer() {}
	SoundPlayer(MSSourceVoice&aVoice,const XAUDIO2_BUFFER&aBuffer);
	void Play();
	void Stop(bool aReStart);
	void SetVolume(float aVolume);
	float Volume()const;
	void SetLoop(const bool aIsLoop);
private:
	MSSourceVoice mVoice;
	XAUDIO2_BUFFER mBuffer;
};