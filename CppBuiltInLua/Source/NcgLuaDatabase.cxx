#include "NcgLuaDatabase.hxx"
#include"NcgLuaManager.hxx"
#include<cassert>



void NcgLuaDatabase::Load(const std::string& aFileName, const int aId, std::string aIdName)
{
	//�w��ID���g�p�ς݂ł���ꍇ
	assert(mLuaDatabase.count(aId) == 0);

	NcgLuaManager* lManager = new NcgLuaManager;
	lManager->Load(aFileName);

	//�}�l�[�W����o�^
	mLuaDatabase.insert({ aId,lManager });
	//����ID���o�^
	if (aIdName.empty() == true) {
		aIdName = aFileName;
	}
	mIdAccesser.insert({ aIdName, aId });
}

NcgLuaManager * NcgLuaDatabase::GetManager(int aId)
{
	NcgLuaManager* lResult{};
	if (mLuaDatabase.count(aId) != 0) {
		lResult = mLuaDatabase[aId];
	}
	return lResult;
}

NcgLuaManager * NcgLuaDatabase::GetManager(const std::string & aIdName)
{
	NcgLuaManager* lResult{};
	if (mIdAccesser.count(aIdName) != 0) {
		lResult = mLuaDatabase[mIdAccesser[aIdName]];
	}
	return lResult;
}

void NcgLuaDatabase::Release()
{
	for (auto& lManager : mLuaDatabase) {
		delete lManager.second;
	}
	mLuaDatabase.clear();
	mIdAccesser.clear();
	
}

NcgLuaDatabase::~NcgLuaDatabase()
{
	Release();
}
