#pragma once
#include<unordered_map>
#include<memory>
#include<random>
class DXTexture;
struct ID3D11Texture2D;
//�e�N�X�`���Ǘ��N���X
class DX11TextureManager {
public:

	~DX11TextureManager();

	//�e�N�X�`���̈ꊇ�o�^�p
	//�e�N�X�`��ID�A���΃t�@�C���p�X
	typedef std::unordered_map<int, std::string>FileList;

	//�e�N�X�`���o�^
	//���Ƀt�@�C�������݂����ꍇ�A�o�^���Ȃ�
	//�f�o�b�O����asset�`�F�b�N
	bool RegistFromFile(const std::string&pFileName, const int pID);

	//�e�N�X�`���o�^
	//���������̃e�N�X�`�������̂܂ܓo�^����
	bool RegistFromMemory(ID3D11Texture2D*&pTexture, const int pID);

	//�e�N�X�`���̈ꊇ�o�^
	//���Ƀt�@�C�������݂����ꍇ�A�o�^���Ȃ�
	//�f�o�b�O����asset�`�F�b�N
	bool RegisterFileList(const FileList&pFileList);

	//�e�N�X�`����ǂݍ���(�Q��)
	//�t�@�C�������݂��Ȃ������ꍇ�A�擾���Ȃ�
	//�f�o�b�O����asset�`�F�b�N
	bool Load(DXTexture*&pDstTexture,const int pID)const;

	//�w�肵��ID�Ƀe�N�X�`�����o�^����Ă��邩�ǂ���
	bool IsRegisterd(const int pID);
	//�o�^�����t�@�C�����폜����
	void UnRegisterFile(const int pID);
	//�o�^�����t�@�C�����ׂĂ��폜����
	void UnRegisterFileAll();

	//���g�p��ID��T��
	int GetNotRegisteredID();
	int GetRegisteredCount();
private:
	//�e�N�X�`��ID�����p����
	std::mt19937 mTextureIDRandom;
	//���lID�̎w���e�N�X�`���f�[�^
	std::unordered_map<int, std::shared_ptr<DXTexture>> mTextureMap;
};
