#pragma once
#include<string>
class MSString :public std::string {
public:
	//�t�@�C���̃t�H���_�p�X���擾����
	static void GetFolderPath(std::string&pDstString, const std::string&pFilePath);
};