//-----------------------------------------------------
//キー入力チェッククラス
//関数Actioned()はキーを押した直後だけtrueを返します(押しっぱなし防止のため)
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