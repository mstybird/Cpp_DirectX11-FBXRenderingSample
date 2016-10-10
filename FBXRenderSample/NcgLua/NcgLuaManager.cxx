#include"NcgLuaManager.hxx"
#include"Lua\lua.hpp"
#include<cassert>
#include<string>
#include<vector>
using std::string;
using std::vector;

/*
	Return���\�b�h�ŌĂ΂��\���̂���^�̂ݖ����I���̉�
	�W���ő��݂��Ȃ��^�͏��������Ȃ�(�ł��Ȃ�)
*/
template void NcgLuaManager::Return(bool& aOutValue, const int aIndex);
template void NcgLuaManager::Return(char& aOutValue, const int aIndex);
template void NcgLuaManager::Return(short& aOutValue, const int aIndex);
template void NcgLuaManager::Return(int& aOutValue, const int aIndex);
template void NcgLuaManager::Return(long& aOutValue, const int aIndex);
template void NcgLuaManager::Return(long long& aOutValue, const int aIndex);
template void NcgLuaManager::Return(unsigned char& aOutValue, const int aIndex);
template void NcgLuaManager::Return(unsigned short& aOutValue, const int aIndex);
template void NcgLuaManager::Return(unsigned int& aOutValue, const int aIndex);
template void NcgLuaManager::Return(unsigned long& aOutValue, const int aIndex);
template void NcgLuaManager::Return(unsigned long long& aOutValue, const int aIndex);
template void NcgLuaManager::Return(float& aOutValue, const int aIndex);
template void NcgLuaManager::Return(double& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::string& aOutValue, const int aIndex);

template void NcgLuaManager::Return(std::vector<bool>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<char>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<short>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<int>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<long>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<long long>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<unsigned char>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<unsigned short>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<unsigned int>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<unsigned long>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<unsigned long long>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<float>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<double>& aOutValue, const int aIndex);
template void NcgLuaManager::Return(std::vector<std::string>& aOutValue, const int aIndex);

//C���C�N�Ɏ擾����B
//��1�߂�l�̃C���f�b�N�X��0�Ƃ��A�ȍ~�A1,2,3�Ƒ���
template<typename tType>
void NcgLuaManager::Return(tType& aOutValue, const int aIndex) {
	//�C���f�b�N�X���͈͊O�������ꍇ�G���[�𔭐�������
	assert(lua_gettop(mState)>aIndex && 0 <= aIndex);
	mHelper->GetReturnValue(aOutValue, aIndex - lua_gettop(mState));
	return;
}

NcgLuaManager::NcgLuaManager()
{
	mFuncName = new std::string;
}

NcgLuaManager::~NcgLuaManager()
{
	delete mFuncName;
}

//�֐����Ƃɖ߂�l�̐��ƈ����̐����`����
void NcgLuaManager::SetFunction(
	const std::string& aFunctionName,
	const int aReturnCount,
	const int aArgsCount
) {
	*mFuncName = aFunctionName;
	mReturnCount = aReturnCount;
	mArgsCount = aArgsCount;
}

void NcgLuaManager::Clone(NcgLuaManager & aOut)
{
	aOut.Release();
	aOut.mArgsCount = this->mArgsCount;
	aOut.mFirstCalled = this->mFirstCalled;
	aOut.mFuncName = this->mFuncName;
	aOut.mHelper = this->mHelper;
	aOut.mLoadedLua = this->mLoadedLua;
	aOut.mLoadedStandardLib= this->mLoadedStandardLib;
	aOut.mReturnCount = this->mReturnCount;
	aOut.mState = this->mState;
	return;
}

void NcgLuaManager::GetFunction(const char* aFuncName)
{
	lua_getglobal(mState, aFuncName);
	return;
}


template bool NcgLuaManager::GetGlobal(const std::string&aValueName, bool& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, char& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, short& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, int& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, long& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, long long& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, unsigned char& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, unsigned short& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, unsigned int& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, unsigned long& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, unsigned long long& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, float& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, double& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, string& aOutValue);


template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<char>& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<short>& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<int>& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<long>& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<long long>& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<unsigned char>& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<unsigned short>& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<unsigned int>& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<unsigned long>& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<unsigned long long>& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<float>& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<double>& aOutValue);
template bool NcgLuaManager::GetGlobal(const std::string&aValueName, vector<string>& aOutValue);

//�l���擾����
template<typename tType>
bool NcgLuaManager::GetGlobal(const std::string&aValueName, tType& aOutValue) {
	return mHelper->GetValue(aValueName, aOutValue);
}
