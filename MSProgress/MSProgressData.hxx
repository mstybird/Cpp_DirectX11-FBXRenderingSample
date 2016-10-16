#pragma once

//�ő�l�ƍŏ��l�A���ݒl��ێ�����N���X
/*
���ݒl�̐ݒ�Ɏ��s�����ꍇ�A�ݒ�ς݂̍ŏ��l���Z�b�g�����
*/

enum class MSPRogressFlag {
	LESSMIN,
	GREATEMAX,
	NOTOVERFLOW
};

class MSProgress
{
public:
	MSProgress();

	float GetMax();
	float GetNow();
	float GetMin();
	//���݂̐��l���ő�l�ɑ΂��ĉ��������ǂ���(100%��1.0f�Ƃ���)
	float GetNowPer();
	//���Ɖ����ōő�l�ɒB���邩(100%��1.0f�Ƃ���)
	float GetMaxRemainPer();
	//���Ƃ�����ōő�l�ɒB���邩
	float GetMaxRemainFix();


	///�ŏ��l���ő�l���傫���ꍇ�A
	///�z�肵�Ă͂����Ȃ�����Ȃ̂ŋ����I������
	//���ꂼ��̒l���Z�b�g����
	void Set(float aMax, float aMin, float aNow);
	//���ݒn�ƂȂ�l�𒼐ڃZ�b�g����
	void SetNowFix(float aNowValue);
	//���ݒn�ƂȂ�l������(1��100%�Ƃ���)���Z�b�g����
	void SetNowPer(float aNowValue);

	//�w�肵���l�����̂܂܉��Z����
	//�͈͂����ꂽ���̏���Ԃ�
	MSPRogressFlag AddFix(float aValue);
	//�w�肵���l�����̂܂܌��Z����
	//�͈͂����ꂽ���̏���Ԃ�
	MSPRogressFlag SubFix(float aValue);
	//�w�肵���l������(1��100%�Ƃ���)�����Z����
	//�͈͂����ꂽ���̏���Ԃ�
	MSPRogressFlag AddPer(float aValue);
	//�w�肵���l������(1��100%�Ƃ���)�����Z����
	//�͈͂����ꂽ���̏���Ԃ�
	MSPRogressFlag SubPer(float aValue);

	MSPRogressFlag IsBetweenValue(float aValue);
protected:
	void FitValue();
protected:
	float mMax;
	float mNow;
	float mMin;
};