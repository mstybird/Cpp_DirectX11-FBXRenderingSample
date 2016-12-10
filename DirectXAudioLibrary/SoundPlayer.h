#pragma once
//#include"SoundFwd.h"
#include"SoundImpl.h"
class SoundImpl;
class SoundPlayer {
public:
	explicit SoundPlayer(MSSourceVoice&aVoice);
	void Play();
	void Stop(bool aReStart);
	void SetVolume(float aVolume);
	float Volume()const;
private:
	MSSourceVoice mVoice;
};