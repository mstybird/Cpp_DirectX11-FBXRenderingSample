#include "Helper.h"

void MSString::GetFolderPath(std::string & pDstString, const std::string & pFilePath)
{
	
	int index = pFilePath.find_last_of('\\');
	if (index == std::string::npos) {
		index = pFilePath.find_last_of('/');
	}
	if (index != std::string::npos) {
		pDstString = pFilePath.substr(0, index);

	}
}
