#pragma once
#include<vector>
#include<cassert>
#include<unordered_map>
#include<type_traits>
#include<random>
struct StatusField;
namespace Dijkstra {
	using namespace std;
	
	class Manager;
	struct Node {
		static const float cNotInitCost;
		friend class Manager;
		Node(int id) {
			this->mId = id;
			this->mSearched = false;
			this->mPreviousNode = nullptr;
			this->mCost = cNotInitCost;
		}
		virtual ~Node() {}

		void addNode(Node*const aNode, const int aCost) {
			mEdgeNode.push_back({ aNode, aCost });
		};

		int GetID() {
			return this->mId;
		}

		int GetCost() {
			return this->mCost;
		}

		void Reset() {
			this->mSearched = false;
			this->mPreviousNode = nullptr;
			this->mCost = cNotInitCost;
		}
	private:
		vector<std::pair<Node*, int>>mEdgeNode;	//隣接ノードの配列
		bool mSearched;			//走査済みかどうか
		Node*mPreviousNode;		//前のノード
		float mCost;			//ノード本体のコスト
		int mId;			//ノードID
	};


	class Manager {
	public:

		Manager() {}
		virtual ~Manager();
		//安全なノード生成
		void CreateNodeSafe(Node* aAddNode);
		//ノードのリンクを作成
		void LinkNode(std::pair<int,int>aFromTo, int aCost, bool aBothFlag);

		//ノードデータの作成
		virtual void CreateNodes(StatusField&aField) = 0;
		//指定したIDの要素を削除する
		void RemoveNode(const int aRemoveID);

		//スタートノードを設定
		void SetStartNode(const int StartNode);
		void SetNowNode(Node* const aNowNode);
		//ノード数取得
		int GetNodeCount()const;
		//存在するノードの中から、ランダムにノードを選出し、
		//そのインデクスを取得する
		//aIncludeNowNodeFlag:false	現在ノードを含まない
		int GetRandomNodeIndex(bool aIncludeNowNodeFlag = false);

		//設定したスタート地点からのルートを計算する
		bool GenerateRoot();

		//現在のノードを取得
		//ノードがない場合はfalseを返し、OutNodeにnullptrが入る
		template<typename T>
		bool GetFrontNode(T*& OutNode) {

			static_assert(std::is_convertible<T*, Node*>::value == true, "Is not Node Object. Use Class(Struct) that Inherits from Node or, Dijkstra::Node");

			if (mRootNodes.size() > 0) {
				OutNode = static_cast<T*>(mRootNodes.back());
				return true;
			}
			else {
				OutNode = nullptr;
				return false;
			}
		}
		
		//ルートを初期化する
		void ClearRoot();

		//次のノードに進める
		//進めるノードがなくなった場合falseを返す
		bool SetNextNode();

		//現在いるノードから直接移動可能なノードをランダムに計算する
		//移動できない場合はfalseを返す
		//BackFlagにtrueを設定した場合、直前ノードには戻らないようにする
		//この際、生成済みのルートは初期化する
		bool CreateNextRoot(bool aBackFlag);
		//現在居るノードを取得する
		Node* GetNowNode();
		//現在のノードから移動可能なノードの数を取得する
		int GetEdgeNodeCount(const Node*aRootNode);

		bool IsEdgeIntoNode(const Node*IntoNodes, const Node* aSearchNode);

		//ルートを構築する
		bool CreateRoot(int aGoalIndex);
		//ノードのコストを初期化する
		void ResetNodes();
		//指定したインデクスが範囲内か調べる
		bool IsBetweenIndex(const int NodeIndex);
		//探索ルートが空だったらtrueを返す
		bool IsRootEmpty();

	protected:
		std::mt19937 mRand;
		bool mIsGenerated = false;
		vector<Node*>mNodes;	//存在するノード
		vector<Node*>mRootNodes;//生成したルートのノード
		Node*mNowNode{};			//現在のノード(終着点)
		Node*mEndFrontNode{};		//終着点ノードの直前ノード
		int mStartIndex = -1;		//スタートノード
	};

}
