#pragma once

//�ő�l�ƍŏ��l�A���ݒl��ێ�����N���X
/*
	���ݒl�̐ݒ�Ɏ��s�����ꍇ�A�ݒ�ς݂̍ŏ��l���Z�b�g�����
*/

enum class MSPRogressFlag{
	LESSMIN,
	GREATEMAX,
	NOTOVERFLOW
};

class MSProgress
{

	MSProgress();

	float GetMax();
	float GetNow();
	float GetMin();

	///�ŏ��l���ő�l���傫���ꍇ�A
	///�z�肵�Ă͂����Ȃ�����Ȃ̂ŋ����I������
	//���ꂼ��̒l���Z�b�g����
	void Set(float aMax, float aMin, float aNow);
	//���ݒn�ƂȂ�l�𒼐ڃZ�b�g����
	bool SetNowFix(float aNowValue);
	//���ݒn�ƂȂ�l������(1��100%�Ƃ���)���Z�b�g����
	bool SetNowPer(float aNowValue);
	
	//�w�肵���l�����̂܂܉��Z����
	//�ő�l�𒴂�����true��Ԃ��A
	//�ő�l�ɖ����Ȃ��ꍇ��false��Ԃ�
	bool AddFix(float aValue);
	//�w�肵���l�����̂܂܌��Z����
	//�ŏ��l�𒴂�����true��Ԃ��A
	//�ŏ��l�ɖ����Ȃ��ꍇ��false��Ԃ�
	bool SubFix(float aValue);
	//�w�肵���l������(1��100%�Ƃ���)�����Z����
	//�ő�l�𒴂�����true��Ԃ��A
	//�ő�l�ɖ����Ȃ��ꍇ��false��Ԃ�
	bool AddPer(float aValue);
	//�w�肵���l������(1��100%�Ƃ���)�����Z����
	//�ŏ��l�𒴂�����true��Ԃ��A
	//�ŏ��l�ɖ����Ȃ��ꍇ��false��Ԃ�
	bool SubPer(float aValue);

	bool IsBetweenValue(float aValue);
protected:
	float mMax;
	float mNow;
	float mMin;
};