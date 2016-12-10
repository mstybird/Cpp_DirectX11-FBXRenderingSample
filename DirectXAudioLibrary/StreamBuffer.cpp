#include "StreamBuffer.h"

namespace {
	OggVorbis_File* CreateOggVorbis(const std::string& aFileName) {
		OggVorbis_File* lOvf{ new OggVorbis_File() };
		FILE* f;
		if (fopen_s(&f, aFileName.c_str(), "rb")) {
			printf("Failed To Open %s\n", aFileName.c_str());
			return nullptr;
		}
		if (ov_open(f, lOvf, nullptr, 0)) {
			fclose(f);
			return nullptr;
		}
		return lOvf;
	}
}

StreamBuffer::StreamBuffer(const std::string & aFileName):
	ovf{CreateOggVorbis(aFileName),OggVorbisDeleter},
	mFormat{}
{
	const vorbis_info *info(ov_info(ovf.get(), -1));
	if (!info) {
		printf("Failed To Get OGG Format.\n");
		return;
	}

	ZeroMemory(&mFormat, sizeof(mFormat));
	//oggファイルから得た情報を使ってWAVEFORMATEXを初期化

	mFormat.wFormatTag = WAVE_FORMAT_PCM;
	mFormat.nChannels = static_cast<WORD>(info->channels);
	mFormat.nSamplesPerSec = info->rate;
	mFormat.wBitsPerSample = 16;
	mFormat.nBlockAlign = static_cast<WORD>(info->channels * 2);
	mFormat.nAvgBytesPerSec = mFormat.nSamplesPerSec*mFormat.nBlockAlign;

}
