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
	//既にテクスチャが存在した場合は読み込まない
	if (mTextureMap[pID].use_count() != 0) {
		lResult = false;
		assert(0 && pID);
	}
	else {
		shared_ptr<DXTexture>texture;
		//指定したIDにテクスチャがなければ読み込み
//		texture.reset(new DXTexture);
		texture = std::make_shared<DXTexture>();
		//読み込めたら登録
		if (texture->Create(pFileName)) {
			mTextureMap[pID] = texture;
		}
		else {
			//読み込めなければ登録しない
			lResult = false;
			assert(0 && pFileName.c_str());
		}
		
	}

	return lResult;
}

bool DX11TextureManager::RegisterFileList(const FileList & pFileList)
{
	//一つでも読み込めないファイルがあればfalseを返す
	bool lResult = true;
	shared_ptr<DXTexture>texture;
	for (auto lFile : pFileList) {
		//既にテクスチャが存在した場合は読み込まない
		if (mTextureMap[lFile.first].use_count() != 0) {
			lResult = false;
			assert(0 && lFile.first);
		}
		else {
			//テクスチャ作成
			//指定したIDにテクスチャがなければ読み込み
			texture.reset(new DXTexture);
			
			//読み込めたら登録
			if (texture->Create(lFile.second)) {
				mTextureMap[lFile.first] = texture;
			}
			else {
				//読み込めなければ登録しない
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
	//読み込めたらtrueを返す
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
