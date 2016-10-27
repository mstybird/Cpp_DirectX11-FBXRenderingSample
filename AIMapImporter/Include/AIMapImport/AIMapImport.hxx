#pragma once
//#include<iostream>
#include<vector>
//��p��AI�}�b�v���C���|�[�g����p

namespace Comfort {

	struct AINodeData {
		int32_t mID;
		std::vector<int32_t>mLinkList;
		union
		{
			struct {
				float_t mX;
				float_t mY;
				float_t mZ;
			};
			float_t mPosition[3];
		};
	};

	class AIMapImporter {
	public:
		//�t�@�C����ǂݍ���
		bool Import(const char*aFilePath);
		//�m�[�h���X�g�̎Q�Ƃ��擾
		const std::vector<AINodeData>* GetList()const;
	private:
		std::vector<AINodeData> mNodeList;
	};

}
