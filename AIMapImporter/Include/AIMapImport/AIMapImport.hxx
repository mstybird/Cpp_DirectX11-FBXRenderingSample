#pragma once
//#include<iostream>
#include<vector>
//専用のAIマップをインポートする用

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
		int32_t mTeamIDType;
	};

	class AIMapImporter {
	public:
		//ファイルを読み込む
		bool Import(const char*aFilePath);
		//ノードリストの参照を取得
		const std::vector<AINodeData>* GetList()const;
	private:
		std::vector<AINodeData> mNodeList;
	};

}
