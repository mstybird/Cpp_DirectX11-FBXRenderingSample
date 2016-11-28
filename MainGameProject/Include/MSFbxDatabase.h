#pragma once
#include<unordered_map>
#include<memory>
class MSFbxManager;
//FBXデータを一元管理する
//シーンごとに独立して扱えるようにシングルトン化はしない
class MSFbxDatabase {
public:
	bool Load(const char* aFileName,const bool aIsAnimation, const int32_t aID);
	MSFbxManager* Get(const int32_t aID)const;
	bool IsExist(const int32_t aID)const;
	void Release();
private:
	std::unordered_map<int32_t, std::unique_ptr<MSFbxManager>> mDatabase;
};

