#include"Lua\lua.hpp"
#include"NcgLuaBase.hxx"
#include"NcgLuaHelper.hxx"
#include<string>
#include<iostream>

NcgLuaBase::NcgLuaBase() {
	mState = luaL_newstate();
	mHelper = new NcgLuaHelper;
	mHelper->SetState(mState);
};
NcgLuaBase::~NcgLuaBase() {
	Release();
	delete mHelper;
};

void NcgLuaBase::Release() {
	lua_close(mState);
	ResetFlag();
}

//�t�@�C���̓ǂݍ���(������Ԃł͎������s
bool NcgLuaBase::Load(const std::string&aFileName, bool aLoadToExec, bool aLoadStandardLib) {
	{
		//�ǂݍ��ݑO�̃t���O�̏�����
		if (mLoadedLua == true)ResetFlag();

		//�t�@�C���ǂݍ���
		if (!this->LoadFile(aFileName)) {
			lua_close(mState);
			this->ResetFlag();
			return false;
		}

		//�ǂݍ��ݐ����t���O���Z�b�g
		mLoadedLua = true;

		//�W�����C�u�����̓ǂݍ���
		if (aLoadStandardLib == true) {
			luaL_openlibs(mState);

			//lua�t�@�C������̑��΃p�X�ǉ�
			size_t path_i = aFileName.find_last_of("/");
			if (path_i == 0) {
				path_i = aFileName.find_last_of("\\") + 1;

			}
			std::string pathname = ";" + aFileName.substr(0, path_i + 1);
			pathname += "?.lua";
			lua_getglobal(mState, "package");
			lua_getfield(mState, -1, "path"); // get field "path" from table at top of stack (-1)
			std::string cur_path = lua_tostring(mState, -1); // grab path string from top of stack
			cur_path.append(";"); // do your path magic here
			cur_path.append(pathname.c_str());
			lua_pop(mState, 1); // get rid of the string on the stack we just pushed on line 5
			lua_pushstring(mState, cur_path.c_str()); // push the new one
			lua_setfield(mState, -2, "path"); // set the field "path" in table at -2 with value at top of stack
			lua_pop(mState, 1); // get rid of package table from top of stack


			mLoadedStandardLib = true;
		}
	}

	//lua�̍ŏ��̎��s
	{
		//�ǂݍ��݂Ɠ����Ɏ��s����ꍇ
		if (aLoadToExec == true) {
			if (!this->CallFunction()) {
				lua_close(mState);
				this->ResetFlag();
				return false;
			}
			//�ŏ��̎��s�����t���O�𗧂Ă�
			mFirstCalled = true;
		}
	}

	return true;
}

bool NcgLuaBase::LoadFile(const std::string&aFileName) {
	//�t�@�C���ǂݍ���
	int lResult = luaL_loadfile(mState, aFileName.c_str());
	//�ǂݍ��݂Ɏ��s������G���[�\������false��Ԃ�
	if (lResult != 0) {
		ErrorLog();
		return false;
	}
	return true;
}

//�X�^�b�N�ɑ��݂���֐��̌Ăяo��
//���������true�A���s�����false���A��
bool NcgLuaBase::CallFunction(int aArgCount, int aReturnCount, int aErrorValue) {
	int lResult = lua_pcall(mState, aArgCount, aReturnCount, aErrorValue);
	if (lResult != 0) {
		ErrorLog();
		return false;
	}
	return true;
}

//�ǂݍ��݊֌W�̃t���O��������
void NcgLuaBase::ResetFlag() {
	mLoadedStandardLib = false;
	mLoadedLua = false;
	mFirstCalled = false;
};

//��������G���[���o���炱�̊֐����Ă�
//aClose��true�ɂ���ƃG���[�o�͂ƂƂ���LuaState�����
//�G���[���Ȃ����false��Ԃ�
void NcgLuaBase::ErrorLog() {
	std::cout << lua_tostring(mState, -1) << std::endl;
}

void NcgLuaBase::ClearStack()
{
	lua_settop(mState, 0);
}
