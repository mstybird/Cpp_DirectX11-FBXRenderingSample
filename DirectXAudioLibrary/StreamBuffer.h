#pragma once
#include"Deleter.hxx"
#include<vorbis\vorbisfile.h>
#include<memory>
#include<string>
#include<XAudio2.h>
struct StreamBuffer {
	explicit StreamBuffer(const std::string &aFileName);
	std::unique_ptr<OggVorbis_File,decltype(&OggVorbisDeleter)>ovf;
	WAVEFORMATEX mFormat;
};