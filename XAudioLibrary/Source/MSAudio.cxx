#include"MSAudio.hxx"

void MSAudio::InitializeSound()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(XAudio2Create(&mXAudio2, XAUDIO2_DEBUG_ENGINE))) {
		CoUninitialize();
		return;
	}
	if (FAILED(mXAudio2->CreateMasteringVoice(&mMasteringVoice))) {
		CoUninitialize();
		return;
	}
	return;
}

void MSAudio::PlayAudio(int aIndex)
{
}

int MSAudio::LoadAudio(const char * aFileName)
{
	return 0;
}

MSAudio::~MSAudio()
{
}
