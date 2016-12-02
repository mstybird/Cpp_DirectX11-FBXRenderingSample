#include"StatusBulletBase.h"
#include<cassert>

BulletDamageType ConvertDamageType(int aType)
{
	BulletDamageType lType;

	switch (aType)
	{
	case 0:
		lType = BulletDamageType::NORMAL;
		break;
	case 1:
		lType = BulletDamageType::PERCENT;
		break;
	case 2:
		lType = BulletDamageType::FIXED;
		break;
	default:
		//�����Ă͂Ȃ�Ȃ��̂Ńf�o�b�O���ɒ��˂�
		assert(0);
		break;
	}

	return BulletDamageType();
}