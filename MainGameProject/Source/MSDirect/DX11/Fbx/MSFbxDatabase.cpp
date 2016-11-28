#include "MSFbxDatabase.h"
#include"MSFbxManager.h"
#include<cassert>
bool MSFbxDatabase::Load(const char * aFileName, const bool aIsAnimation, const int32_t aID)
{
	//存在している場合は追加不可
	//バグの原因となるのであってはならないこととする
	assert(IsExist(aID) == 0);
	printf("FBX: Register %s\n", aFileName);
	decltype(auto) lFbx = std::make_unique<MSFbxManager>();
	lFbx->LoadFile(aFileName, aIsAnimation);
	mDatabase[aID] = std::move(lFbx);

	return false;
}

MSFbxManager * MSFbxDatabase::Get(const int32_t aID) const
{
	//存在していればそのリソースを返す
	if (IsExist(aID)) {
		return mDatabase.at(aID).get();
	}
	else {
		return nullptr;
	}
}

bool MSFbxDatabase::IsExist(const int32_t aID) const
{
	return mDatabase.count(aID) != 0;
}

void MSFbxDatabase::Release()
{
	for (auto&lFbx : mDatabase) {
		lFbx.second->Release();
	}
}
