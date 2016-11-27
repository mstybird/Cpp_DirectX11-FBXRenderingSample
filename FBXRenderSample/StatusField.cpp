#include "StatusField.h"
#include"DXWorld.h"
#include"DXMatrix.h"
#include"AI\EnemyAI.h"
#include"CharacterBase.h"
#include<SpawnMapImporter.hpp>
#include<AIMapImport.hpp>
#include<cassert>
#include<string>
StatusField::~StatusField()
{
	for (decltype(auto)lNode : mFieldNodes) {
		delete lNode;
	}
	for (decltype(auto)lNode : mSpawnBallNodes) {
		delete lNode;
	}
}
void StatusField::CreateFieldNodes()
{
	Comfort::AIMapImporter im;
	im.Import("sample.anm");
	auto v = im.GetList();
	using NodeControl::AddNodeSafe;
	//ノードの追加
	DXVector3 lNodePosition{};
	for (decltype(auto)lData : *v) {
		lNodePosition.Set(-lData.mX, lData.mY, lData.mZ);
		AddNodeSafe(
			mFieldNodes,
			new MyNode{
				lData.mID, 
				std::string("obj") +std::to_string(lData.mID),lNodePosition,lData.mTeamIDType
			}
		);
	}


	//ノード間固定コストは距離とする						
	auto LinkNodeSafeCustom = [this](const int32_t aFromID, const int32_t aToID, const bool aIsBothFlag) {
		using NodeControl::LinkNodeSafe;
		//ノード間の距離を得る					
		auto NodeLength = [this](const int32_t aFromID, const int32_t aToID) {
			auto& lFromPos = static_cast<MyNode*>(mFieldNodes[aFromID])->Position;
			auto& lToPos = static_cast<MyNode*>(mFieldNodes[aToID])->Position;
			return (lFromPos - lToPos).GetDistance();
		};
		LinkNodeSafe(mFieldNodes, { aFromID,aToID }, NodeLength(aFromID, aToID), aIsBothFlag);
	};

	//リンクノードの追加
	for (decltype(auto)lNode : *v) {
		printf("%d\n", lNode.mID);
		for (decltype(auto)lLink : lNode.mLinkList) {
			LinkNodeSafeCustom(lNode.mID, lLink, true);
		}
	}


}
void StatusField::CreateSpawnCharaNodes()
{
	//黒チームのスポーンデータ
	{
		Comfort::SpawnMapImporter lImport;
		lImport.Import("res/NodeMap/spawnTeamBlack.anm");


		using NodeControl::AddNodeSafe;

		for (const auto& lNode : *lImport.GetList()) {
			AddNodeSafe(
				mTeamBlack.mSpawnCharaNodes,
				new MyNode{
				lNode.mID,
				std::string("obj") + std::to_string(lNode.mID),
				{ -lNode.mX,lNode.mY,lNode.mZ },
				0
			}
			);
		}
	}


	//黒チームのスポーンデータ
	{
		Comfort::SpawnMapImporter lImport;
		lImport.Import("res/NodeMap/spawnTeamWhite.anm");


		using NodeControl::AddNodeSafe;

		for (const auto& lNode : *lImport.GetList()) {
			AddNodeSafe(
				mTeamWhite.mSpawnCharaNodes,
				new MyNode{
				lNode.mID,
				std::string("obj") + std::to_string(lNode.mID),
				{ -lNode.mX,lNode.mY,lNode.mZ },
				0
			}
			);
		}
	}

	//AddNodeSafe(mSpawnCharaNodes, new MyNode{ 19,"obj19",{ 11.00,0.00,7.50 } });
	//AddNodeSafe(mSpawnCharaNodes, new MyNode{ 20,"obj20",{ -16.25,0.00,5.25 } });
}
void StatusField::CreateSpawnBallNodes()
{
	Comfort::SpawnMapImporter lImport;
	lImport.Import("res/NodeMap/spawnBall.anm");


	using NodeControl::AddNodeSafe;

	for (const auto& lNode : *lImport.GetList()) {
		AddNodeSafe(
			mSpawnBallNodes,
			new MyNode{
				lNode.mID,
				std::string("obj") + std::to_string(lNode.mID),
				{-lNode.mX,lNode.mY,lNode.mZ},
				0
			}
		);
	}

	//AddNodeSafe(mSpawnBallNodes, new MyNode{ 0,"obj0",{ -16.40,0.00,- 47.00 }  ,0 });
	//AddNodeSafe(mSpawnBallNodes, new MyNode{ 5,"obj5",{ 6.25,0.00,2.50 } });
	//AddNodeSafe(mSpawnBallNodes, new MyNode{ 13,"obj13",{ -4.00,0.00,8.50 } });
}
void StatusField::InitGoalIndex()
{
	mTeamWhite.mGoalIndex = 19;
	mTeamBlack.mGoalIndex = 80;
}
std::vector<Dijkstra::Node*> StatusField::GetFieldNodesClone()
{
	return mFieldNodes;
}
void StatusField::Respawn(CharacterBase * aSpawnChara)
{
	printf("%s is Respawn\n", typeid(aSpawnChara).name());

	//チームのスポーンリスト取得
	const auto& lSpawnNodeList = GetTeamAlly(aSpawnChara)->mSpawnCharaNodes;

	auto lCount = rand() % lSpawnNodeList.size();

	//ランダムなノードから座標を取り出す
	auto& lPosition = static_cast<MyNode*>(lSpawnNodeList[lCount])->Position;

	aSpawnChara->GetWorld()->SetT(lPosition);
	aSpawnChara->SetActive(true);
	aSpawnChara->InitStatus();
	
}
void StatusField::RegisterTeamMember(CharacterBase * aRegistMember, eTeamType aType)
{
	switch (aType)
	{
	case eTeamType::White:
		mTeamWhite.mMembers.push_back(aRegistMember);
		break;
	case eTeamType::Black:
		mTeamBlack.mMembers.push_back(aRegistMember);
		break;
	default:
		break;
	}
}
StatusTeam * StatusField::GetTeamAlly(CharacterBase * aMember)
{
	if (aMember == nullptr)return nullptr;


	for (decltype(auto)lMember : mTeamBlack.mMembers) {
		if (lMember == aMember) {
			return &mTeamBlack;
		}
	}
	for (decltype(auto)lMember : mTeamWhite.mMembers) {
		if (lMember == aMember) {
			return &mTeamWhite;
		}
	}
	return nullptr;
}
StatusTeam * StatusField::GetTeamEnemy(CharacterBase * aMember)
{
	if (aMember == nullptr)return nullptr;

	for (decltype(auto)lMember : mTeamBlack.mMembers) {
		if (lMember == aMember) {
			return &mTeamWhite;
		}
	}
	for (decltype(auto)lMember : mTeamWhite.mMembers) {
		if (lMember == aMember) {
			return &mTeamBlack;
		}
	}
	return nullptr;
}
StatusField::StatusField():
	mBallHoldChara{nullptr},
	mBallIsField{false},
	mBall{nullptr},
	mTeamBlack(eTeamType::Black),
	mTeamWhite(eTeamType::White)
{
}

void StatusField::SetBallHolder(CharacterBase * pBallHolder)
{
	mBallHoldChara = pBallHolder;
	mBallIsField = false;
	mBall->SetActive(false);
}

void StatusField::RespawnBall(DXVector3*pPosition)
{
	mBall->SetActive(true);
	mBallIsField = true;
	mBallHoldChara = nullptr;

	if (pPosition != nullptr) {
		mBall->GetWorld()->SetT(*pPosition);
	}
	else {
		//位置が不定の場合、
		//ボールがスポーン可能な場所ランダムにスポーンさせる
		auto lCount = rand()%mSpawnBallNodes.size();
		auto& lPosition =static_cast<MyNode*>(mSpawnBallNodes[lCount])->Position;
		mBall->GetWorld()->SetT(lPosition);
	}
}

void NodeControl::AddNodeSafe(std::vector<Dijkstra::Node*>& aNodeList, Dijkstra::Node * aAddNode)
{
	//NULLチェック
	assert(aAddNode != nullptr);
	
	//ID重複チェック
	//本来あってはならないので、強制終了させる
	for (const auto&lNode : aNodeList) {
		assert(lNode->GetID() != aAddNode->GetID());
	}
	aNodeList.push_back(aAddNode);
}

void NodeControl::LinkNodeSafe(std::vector<Dijkstra::Node*>& aNodeList, std::pair<int, int> aFromTo, float aCost, bool aBothFlag)
{
	assert(IsBetweenIndex<Dijkstra::Node*>(aNodeList,aFromTo.first));
	assert(IsBetweenIndex<Dijkstra::Node*>(aNodeList,aFromTo.second));
	assert(aCost > 0);
	aNodeList[aFromTo.first]->addNode(aNodeList[aFromTo.second], aCost);

	//双方向リンクの場合、ToからFromに対してもリンクする
	if (aBothFlag == true) {
		aNodeList[aFromTo.second]->addNode(aNodeList[aFromTo.first], aCost);
	}
}
