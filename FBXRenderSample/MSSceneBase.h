#pragma once

//�Q�[���V�[��
enum class MSKEY;
class MSSceneBase abstract{
public:
	//�V�[���̈�x�݂̂̏�����
	virtual void Initialize() {};
	//�V�[�������������t���O�Z�b�g
	virtual void InitalizeEnd()final {};
	//�t���[���̍X�V����
	virtual void Update() {};
	//�`�揈��
	virtual void Render() {};
	//�V�[���j������
	virtual void Destroy() {};

	
	virtual ~MSSceneBase() {};

	//�C�x���g�v���V�[�W��
	virtual void KeyDown(MSKEY pKey) {};
	virtual void KeyHold(MSKEY pKey) {};
	virtual void KeyUp(MSKEY pKey) {};
private:
	//�V�[����������������true�ɂȂ�
	bool bInitializeFlag;
};
