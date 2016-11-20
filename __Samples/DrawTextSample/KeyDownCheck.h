//-----------------------------------------------------
//�L�[���̓`�F�b�N�N���X
//�֐�Actioned()�̓L�[�����������ゾ��true��Ԃ��܂�(�������ςȂ��h�~�̂���)
//	2010.02.05 TMO
//-----------------------------------------------------

#pragma once

class KeyDownChk
{
	bool isDown;
	int vKey;

public:
	KeyDownChk()
	{
		vKey = 0;
		isDown = false;
	}

	KeyDownChk( int tgt )
	{
		vKey = tgt;
		isDown = false;
	}

	void SetKey( int tgt )
	{
		vKey = tgt;
		isDown = false;
	}

	bool Actioned()
	{		
		if ( GetAsyncKeyState( vKey ) & 0x8000 ) {
			if ( !isDown  ){
				isDown = true;
				return true;
			}
		} else {
			isDown = false;	
		}
		
		return false;
	}
};