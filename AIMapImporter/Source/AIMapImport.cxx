#include "AIMapImport.hxx"
#include<fstream>
#include<string>
#include<sstream>
namespace Comfort {

	//������𕪊�����
	template<typename T>
	std::vector<T> Split(const std::string& aString, const char aElem) {
		std::vector<T> lResult{};
		std::stringstream lSs(aString);
		T lTempData{};
		while (lSs) {
			lSs >> lTempData;
			lResult.push_back(lTempData);
			lSs.ignore();
		}
		return std::move(lResult);
	}



	bool AIMapImporter::Import(const char * aFilePath)
	{
		std::ifstream lStream(aFilePath);
		//�ǂݍ��߂Ȃ������ꍇ��false��Ԃ�
		if (lStream.fail())return false;

		//������𐔒l�ɕ�������
		

		//�ǂݍ��ݏI���܂ő�����
		AINodeData lNodeData;
		std::string lLinkListStr;
		std::string lPositionStr;
		std::vector<int32_t> lLinkList;
		std::vector<float_t> lPositionList;

		while (lStream) {
			//�m�[�hID�擾
			lStream >> lNodeData.mID;
			lStream >> lLinkListStr;
			//�����N�m�[�h�擾
			lNodeData.mLinkList = Split<int32_t>(lLinkListStr, ',');
			//�����N�m�[�h���Ȃ��ꍇ
			//�T�C�Y��0�ɌŒ肷��
			if (lNodeData.mLinkList.size() == 0 || lNodeData.mLinkList[0] == -1) {
				lNodeData.mLinkList.clear();
			}

			//���W�擾
			lStream >> lNodeData.mX >> lNodeData.mY >> lNodeData.mZ;
			lStream >> lNodeData.mTeamIDType;
			mNodeList.push_back(std::move(lNodeData));
		}
		mNodeList.pop_back();

		return true;
	}

	const std::vector<AINodeData>* AIMapImporter::GetList() const
	{
		return &mNodeList;
	}
}