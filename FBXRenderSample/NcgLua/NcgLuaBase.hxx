#pragma once
#include<string>
#include"NcgLuaHelper.hxx"
class NcgLuaBase {
public:
	enum class eType {
		NIL,
		BOOLEAN,
		LIGHTUSERDATA,
		NUMBER,
		STRING,
		TABLE,
		FUNCTION,
		USERDATA,
		THREAD
	};
	//LuaState�̍쐬
	NcgLuaBase();
	virtual ~NcgLuaBase();

	void Release();

	//�t�@�C���̓ǂݍ���(������Ԃł͎������s
	bool Load(const std::string&aFileName, bool aLoadToExec = true, bool aLoadStandardLib = true);
protected:
	bool LoadFile(const std::string&aFileName);

	//�X�^�b�N�ɑ��݂���֐��̌Ăяo��
	//���������true�A���s�����false���A��
	bool CallFunction(int aArgCount = 0, int aReturnCount = 0, int aErrorValue = 0);

	//�ǂݍ��݊֌W�̃t���O��������
	void ResetFlag();

	//��������G���[���o���炱�̊֐����Ă�
	//aClose��true�ɂ���ƃG���[�o�͂ƂƂ���LuaState�����
	//�G���[���Ȃ����false��Ԃ�
	void ErrorLog();

	//�������X�^�b�N�ɓ����
	template<typename tFirst, typename... tRest>
	void PushArgment(const tFirst& aFirst, const tRest&... aRest) {
		mHelper->PushArgment(aFirst);
		PushArgment(aRest...);
	}
	void PushArgment() {};

	void ClearStack();

	NcgLuaHelper* mHelper;
	lua_State *mState;
	bool mLoadedStandardLib = false;
	bool mLoadedLua = false;
	bool mFirstCalled = false;
};
