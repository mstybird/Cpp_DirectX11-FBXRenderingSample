#pragma once


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

struct IXAudio2MasteringVoice;
struct IXAudio2SourceVoice;
struct IXAudio2;
struct OggVorbis_File;

void IXAudio2Deleter(IXAudio2*aPtr);
void VoiceDeleter(IXAudio2SourceVoice*aPtr);
void MasteringVoiceDeleter(IXAudio2MasteringVoice*aPtr);
void OggVorbisDeleter(OggVorbis_File*aPtr);