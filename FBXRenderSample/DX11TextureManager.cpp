#include "DX11TextureManager.h"
#include"DX11Texture.h"
#include<cassert>
using std::shared_ptr;
using std::string;
DX11TextureManager::~DX11TextureManager()
{
	UnRegisterFileAll();
	mTextureMap.clear();
}
bool DX11TextureManager::RegisterFile(const string & pFileName, const int pID)
{
	bool lResult = true;
	//���Ƀe�N�X�`�������݂����ꍇ�͓ǂݍ��܂Ȃ�
	if (mTextureMap[pID].use_count() != 0) {
		lResult = false;
		assert(0 && pID);
	}
	else {
		shared_ptr<DXTexture>texture;
		//�w�肵��ID�Ƀe�N�X�`�����Ȃ���Γǂݍ���
//		texture.reset(new DXTexture);
		texture = std::make_shared<DXTexture>();
		//�ǂݍ��߂���o�^
		if (texture->Create(pFileName)) {
			mTextureMap[pID] = texture;
		}
		else {
			//�ǂݍ��߂Ȃ���Γo�^���Ȃ�
			lResult = false;
			//assert(0 && pFileName.c_str());
		}
		
	}

	return lResult;
}

bool DX11TextureManager::RegisterFileList(const FileList & pFileList)
{
	//��ł��ǂݍ��߂Ȃ��t�@�C���������false��Ԃ�
	bool lResult = true;
	shared_ptr<DXTexture>texture;
	for (auto lFile : pFileList) {
		//���Ƀe�N�X�`�������݂����ꍇ�͓ǂݍ��܂Ȃ�
		if (mTextureMap[lFile.first].use_count() != 0) {
			lResult = false;
			assert(0 && lFile.first);
		}
		else {
			//�e�N�X�`���쐬
			//�w�肵��ID�Ƀe�N�X�`�����Ȃ���Γǂݍ���
			texture.reset(new DXTexture);
			
			//�ǂݍ��߂���o�^
			if (texture->Create(lFile.second)) {
				mTextureMap[lFile.first] = texture;
			}
			else {
				//�ǂݍ��߂Ȃ���Γo�^���Ȃ�
				lResult = false;
				assert(0 && lFile.second.c_str());
			}

		}

	}

	return false;
}

bool DX11TextureManager::Load(DXTexture*& pDstTexture, const int pID)const
{
	pDstTexture = mTextureMap.at(pID).get();
	//�ǂݍ��߂���true��Ԃ�
	return !pDstTexture;
}

void DX11TextureManager::UnRegisterFile(const int pID)
{
	mTextureMap.erase(pID);
}

void DX11TextureManager::UnRegisterFileAll()
{
	mTextureMap.clear();
}

int DX11TextureManager::GetNotRegisteredID()
{
	int lNotUseID;
	do {
		lNotUseID = mTextureIDRandom();
	} while (mTextureMap[lNotUseID].use_count() != 0);
	return lNotUseID;
}

int DX11TextureManager::GetRegisteredCount()
{
	return mTextureMap.size();
}
