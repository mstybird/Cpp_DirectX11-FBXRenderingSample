#pragma once

//�Q�[���V�[��
class MSSceneBase abstract{
public:
	virtual void Initialize() {};
	virtual void InitalizeEnd()final {};
	virtual void Update() {};
	virtual void Render() {};
	virtual void Destroy() {};
	virtual ~MSSceneBase() {};
	virtual void KeyDown(int pKey) {};
private:
	//�V�[����������������true�ɂȂ�
	bool bInitializeFlag;
};
