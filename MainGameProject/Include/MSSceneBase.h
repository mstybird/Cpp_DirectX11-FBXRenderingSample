#pragma once
#include<Windows.h>

enum class MouseType {
	Left,
	Center,
	Right
};

//�Q�[���V�[��
enum class MSKEY;
class MSSceneBase abstract{
public:
	//�V�[���̈�x�݂̂̏�����
	virtual void Initialize() {};
	//�V�[�������������t���O�Z�b�g
	virtual void InitializeEnd()final { bInitializeFlag = true; };
	//�t���[���̍X�V����
	virtual void Update() {};
	//�`�揈��
	virtual void Render() {};
	//�V�[���j������
	virtual void Destroy() {};

	
	virtual ~MSSceneBase() {};


	virtual bool IsInitialized()final { return bInitializeFlag; }

	//�C�x���g�v���V�[�W��
	virtual void KeyDown(MSKEY pKey) {};
	virtual void KeyHold(MSKEY pKey) {};
	virtual void KeyUp(MSKEY pKey) {};
	virtual void MouseMove(const POINT & aNowPosition, const POINT & aDiffPosition) {};
	virtual void MouseDown(const MouseType aType) {};
	virtual void MouseUp(const MouseType aType) {};
private:
	//�V�[����������������true�ɂȂ�
	bool bInitializeFlag = false;
};
