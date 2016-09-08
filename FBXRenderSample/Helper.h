#pragma once
#include<string>
class MSString :public std::string {
public:
	//ファイルのフォルダパスを取得する
	static void GetFolderPath(std::string&pDstString, const std::string&pFilePath);
};

