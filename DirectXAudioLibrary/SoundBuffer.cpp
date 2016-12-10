#include "SoundBuffer.h"
#include<fstream>

namespace {
	const char cChunkRIFF[4] = { 'R','I','F','F' };
	const char cChunkWAVE[4] = { 'W','A','V','E' };
	const char cChunkFMT[4] = { 'f','m','t',' ' };
	const char cChunkDATA[4] = { 'd','a','t','a' };
	const uint32_t cExHeadSize = 16;
}

SoundBuffer::SoundBuffer() :
	mFormat{ 0 }
{
	
}

bool SoundBuffer::LoadFile(const std::string & aFileName)
{
	std::ifstream fin{ aFileName,std::ios::in | std::ios::binary };
	if (!fin) {
		printf("Faild To Open %s\n", aFileName.c_str());
		return false;
	}

	char lReadChunk[4];
	
	fin.read(lReadChunk, sizeof(lReadChunk));
	
	if (std::memcmp(cChunkRIFF, lReadChunk, sizeof(lReadChunk))) {
		return printf("Not Found For RIFF Chunk\n");
	}
	
	//Skipping For 4 Bytes.
	fin.ignore(4);

	//����4�o�C�g��WAVE�����ׂ�
	fin.read(lReadChunk, sizeof(lReadChunk));
	if (std::memcmp(cChunkWAVE, lReadChunk, sizeof(cChunkWAVE))) {
		return printf("Nor Found For WAVE Chunk\n");
	}

	//����4�o�C�g��fmt �����ׂ�
	fin.read(lReadChunk, sizeof(lReadChunk));
	if (std::memcmp(cChunkFMT, lReadChunk, sizeof(cChunkFMT))) {
		return printf("Nor Found For fmt Chunk\n");
	}

	uint32_t lFormatSize;
	fin.read(reinterpret_cast<char*>(&lFormatSize), sizeof(lFormatSize));
	
	uint16_t lTagId;
	//Reading For Format Tag ID.
	fin.read(reinterpret_cast<char*>(&lTagId), sizeof(lTagId));

	switch (lTagId)
	{
	case WAVE_FORMAT_PCM://Fall Throw
	case WAVE_FORMAT_ADPCM:
		mFormat.wFormatTag = lTagId;
		break;
	default:
		printf("Not Supported Formatting\n");
		return false;
		break;
	}

	// �`�����l������ǂݍ���
	fin.read(reinterpret_cast<char *>(&mFormat.nChannels), sizeof(mFormat.nChannels));
	// �T���v�����[�g��ǂݍ���
	fin.read(reinterpret_cast<char *>(&mFormat.nSamplesPerSec), sizeof(mFormat.nSamplesPerSec));
	// �f�[�^���x��ǂݍ���
	fin.read(reinterpret_cast<char *>(&mFormat.nAvgBytesPerSec), sizeof(mFormat.nAvgBytesPerSec));
	// �u���b�N�T�C�Y��ǂݍ���
	fin.read(reinterpret_cast<char *>(&mFormat.nBlockAlign), sizeof(mFormat.nBlockAlign));
	// �T���v��������̃r�b�g����ǂݍ���
	fin.read(reinterpret_cast<char *>(&mFormat.wBitsPerSample), sizeof(mFormat.wBitsPerSample));

	//�g���w�b�_����ǂݔ�΂�
	fin.ignore(lFormatSize - cExHeadSize);

	//����4�o�C�g��data�����ׂ�
	fin.read(lReadChunk, sizeof(lReadChunk));
	if (std::memcmp(cChunkDATA, lReadChunk, sizeof(cChunkDATA))) {
		return printf("Nor Found For data Chunk\n");
	}

	uint32_t lWaveSize;
	//�g�`�f�[�^�̃T�C�Y��ǂݍ���
	fin.read(reinterpret_cast<char *>(&lWaveSize), sizeof(lWaveSize));
	mBuffer.resize(lWaveSize);
	// �g�`�f�[�^�ǂݍ���
	fin.read(&mBuffer.front(), lWaveSize);

	return true;
}
