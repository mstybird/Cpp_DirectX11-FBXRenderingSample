#include "Deleter.hxx"
#include<cassert>
#include<XAudio2.h>
void IXAudio2Deleter(IXAudio2 * aPtr)
{
	if (aPtr != nullptr) {
		aPtr->Release();
	}
}

void VoiceDeleter(IXAudio2SourceVoice * aPtr)
{
	if (aPtr != nullptr) {
		aPtr->Stop();
		aPtr->DestroyVoice();
	}
}

void MasteringVoiceDeleter(IXAudio2MasteringVoice * aPtr)
{
	if (aPtr != nullptr) {
		aPtr->DestroyVoice();
	}
}
