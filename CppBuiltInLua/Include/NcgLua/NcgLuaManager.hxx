#pragma once
#include"NcgLuaBase.hxx"
#include"NcgUtility.hxx"
class NcgLuaDatabase;
class NcgLuaManager :public NcgLuaBase {
	friend class NcgLuaDatabase;
protected:
	NcgLuaManager();
	~NcgLuaManager();

public:

	//�֐��𒼐ڌĂяo��
	//�Ăяo���ɐ���������true��Ԃ����A���̎��_�Ŋ֐����̖̂߂�l�͎擾�ł��Ȃ�
	template<typename...tArg>
	__declspec(deprecated("�����`�F�b�N���s���܂���BCall���\�b�h���g�p���Ă�������")) bool CallDirect(const std::string&aFuncName, const int aReturnCount, const tArg&...aArgs) {
		GetFunction(aFuncName.c_str());
		PushArgment(aArgs...);
		//�����̌����擾
		int lArgCount = sizeof...(aArgs);
		if (!this->CallFunction(lArgCount, aReturnCount)) {
			this->ErrorLog();
			return false;
		}
		return true;
	}

	//���炩���ߐݒ肵���l�Ŋ֐��R�[��
	template<typename...tArg>
	bool Call(const tArg&... aArgs) {
		//�Ԉ�������̈�����^����ƒ�~������
		NcgAssert(sizeof...(aArgs) == mArgsCount, "Argment Count Error");
		//�ݒ肳��Ă���
		this->ClearStack();
		this->GetFunction(mFuncName->c_str());
		PushArgment(aArgs...);
		if (!this->CallFunction(mArgsCount, mReturnCount)) {
			this->ErrorLog();
			return false;
		}
		return true;
	}
	//C���C�N�Ɏ擾����B
	//��1�߂�l�̃C���f�b�N�X��0�Ƃ��A�ȍ~�A1,2,3�Ƒ���
	template<typename tType>
	void Return(tType& aOutValue, const int aIndex);

	//�֐����Ƃɖ߂�l�̐��ƈ����̐����`����
	void SetFunction(
		const std::string& aFunctionName,
		const int aReturnCount = 0,
		const int aArgsCount = 0
	);

	//�l���擾����
	template<typename tType>
	bool GetGlobal(const std::string&aValueName, tType& aOutValue);

	//�s���S�ȃN���[���B�C����������(�C�����K�v�ɂȂ�����)
	void Clone(NcgLuaManager&aOut);

private:
	void GetFunction(const char*aFuncName);

	std::string* mFuncName;
	//���炩���ߒ�`���Ă���(lua���̉ϒ������ɂ͑Ή����Ȃ�)
	//���Ƃ�static_assert�Ŗ}�~�X���Ȃ���
	int mReturnCount;
	int mArgsCount;

};

