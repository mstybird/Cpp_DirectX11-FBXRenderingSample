#pragma once
#include<vector>
#include<cassert>
#include<unordered_map>
#include<type_traits>
#include<random>
using namespace std;

namespace Dijkstra {
	class Manager;
	struct Node {
		static const int cNotInitCost;
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
		vector<std::pair<Node*, int>>mEdgeNode;	//�אڃm�[�h�̔z��
		bool mSearched;			//�����ς݂��ǂ���
		Node*mPreviousNode;		//�O�̃m�[�h
		int mCost;				//�m�[�h�{�̂̃R�X�g
		int mId;			//�m�[�hID
	};


	class Manager {
	public:

		Manager() {}
		virtual ~Manager();
		//���S�ȃm�[�h����
		void CreateNodeSafe(Node* aAddNode);
		//�m�[�h�̃����N���쐬
		void LinkNode(std::pair<int,int>aFromTo, int aCost, bool aBothFlag);

		//�m�[�h�f�[�^�̍쐬
		virtual void CreateNodes() = 0;
		//�w�肵��ID�̗v�f���폜����
		void RemoveNode(const int aRemoveID);

		//�X�^�[�g�m�[�h��ݒ�
		void SetStartNode(const int StartNode);
		void SetNowNode(Node* const aNowNode);
		//�m�[�h���擾
		int GetNodeCount()const;
		//���݂���m�[�h�̒�����A�����_���Ƀm�[�h��I�o���A
		//���̃C���f�N�X���擾����
		//aIncludeNowNodeFlag:false	���݃m�[�h���܂܂Ȃ�
		int GetRandomNodeIndex(bool aIncludeNowNodeFlag = false);

		//�ݒ肵���X�^�[�g�n�_����̃��[�g���v�Z����
		bool GenerateRoot();

		//���݂̃m�[�h���擾
		//�m�[�h���Ȃ��ꍇ��false��Ԃ��AOutNode��nullptr������
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
		
		//���̃m�[�h�ɐi�߂�
		//�i�߂�m�[�h���Ȃ��Ȃ����ꍇfalse��Ԃ�
		bool SetNextNode();

		//���݂���m�[�h���璼�ڈړ��\�ȃm�[�h�������_���Ɍv�Z����
		//�ړ��ł��Ȃ��ꍇ��false��Ԃ�
		//BackFlag��true��ݒ肵���ꍇ�A���O�m�[�h�ɂ͖߂�Ȃ��悤�ɂ���
		//���̍ہA�����ς݂̃��[�g�͏���������
		bool CreateNextRoot(bool aBackFlag);
		//���݂̃m�[�h����ړ��\�ȃm�[�h�̐����擾����
		int GetEdgeNodeCount(const Node*aRootNode);

		bool IsEdgeIntoNode(const Node*IntoNodes, const Node* aSearchNode);

		//���[�g���\�z����
		bool CreateRoot(int aGoalIndex);
		//�m�[�h�̃R�X�g������������
		void ResetNodes();
		//�w�肵���C���f�N�X���͈͓������ׂ�
		bool IsBetweenIndex(const int NodeIndex);
		//�T�����[�g���󂾂�����true��Ԃ�
		bool IsRootEmpty();

	protected:
		std::mt19937 mRand;
		bool mIsGenerated = false;
		vector<Node*>mNodes;	//���݂���m�[�h
		vector<Node*>mRootNodes;//�����������[�g�̃m�[�h
		Node*mNowNode{};			//���݂̃m�[�h(�I���_)
		Node*mEndFrontNode{};		//�I���_�m�[�h�̒��O�m�[�h
		int mStartIndex = -1;		//�X�^�[�g�m�[�h
	};

}
