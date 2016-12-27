#include "StatusField.h"
#include"DXWorld.h"
#include"DXMatrix.h"
#include"EnemyAI.h"
#include"CharacterBase.h"
#include"StatusBase.h"
#include"StatusEnemy.h"
#include"Enemy.h"
#include"DX11RenderResource.h"
#include"MyScene.h"
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
void StatusField::Initialize()
{
	mTeamBlack.GetBase()->Initialize();
	mTeamWhite.GetBase()->Initialize();
	mTeamBlack.InitScore();
	mTeamWhite.InitScore();
}
void StatusField::InitializeTime(const int aLimitSecond)
{
	mLimitTime = aLimitSecond;
	mRemainTime = aLimitSecond;
}
void StatusField::InitEffect(::Comfort::EfkManager * aManager, ::Comfort::EffectDatabase * aDb, const int aBallGetID, const int aKillID)
{
	auto lEffect = aDb->Get(aBallGetID);
	mEfkBallGet.SetManager(aManager);
	mEfkBallGet.SetEffect(lEffect);
	lEffect = aDb->Get(aKillID);
	mEfkGameSet.SetManager(aManager);
	mEfkGameSet.SetEffect(lEffect);
}
void StatusField::InitRenderAndShader(MS3DRender & aRender, MSBase3DShader & aShader)
{
	mTeamBlack.GetBase()->SetShader(&aShader);
	mTeamWhite.GetBase()->SetShader(&aShader);
	mTeamBlack.GetBase()->SetRenderer(&aRender);
	mTeamWhite.GetBase()->SetRenderer(&aRender);
}
void StatusField::CreateFieldNodes(const std::string& aAIMap)
{
	Comfort::AIMapImporter im;
	im.Import(aAIMap.c_str());
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
void StatusField::CreateSpawnCharaNodes(const std::string& aSpawnMapBlack, const std::string& aSpawnMapWhite)
{
	//黒チームのスポーンデータ
	{
		Comfort::SpawnMapImporter lImport;
		lImport.Import(aSpawnMapBlack.c_str());


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
		lImport.Import(aSpawnMapWhite.c_str());


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
void StatusField::CreateSpawnBallNodes(const std::string& aBallMap)
{
	Comfort::SpawnMapImporter lImport;
	lImport.Import(aBallMap.c_str());


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
void StatusField::InitGoalIndex(const int aWhiteGoalIndex, const int aBlackGoalIndex)
{
	mTeamWhite.SetGoalIndex(aWhiteGoalIndex);
	mTeamBlack.SetGoalIndex(aBlackGoalIndex);
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
	aSpawnChara->InitStatus(aSpawnChara->GetDefaultStatus());
	aSpawnChara->GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimIdle);
	aSpawnChara->GetTransform()->GetMesh()->SetLoopFlag(true);
	aSpawnChara->GetTransform()->GetMesh()->SetFrontFrame();
	
}
void StatusField::RegisterTeamMember(CharacterBase * aRegistMember, eTeamType aType)
{
	switch (aType)
	{
	case eTeamType::White:
		mTeamWhite.GetMembers()->push_back(aRegistMember);
		break;
	case eTeamType::Black:
		mTeamBlack.GetMembers()->push_back(aRegistMember);
		break;
	default:
		break;
	}
}
StaticObject * StatusField::GetTeamBase(eTeamType aTeamType)
{
	StaticObject* lTeamBase{ nullptr };
	switch (aTeamType)
	{
	case eTeamType::White:
		lTeamBase = mTeamWhite.GetBase();
		break;
	case eTeamType::Black:
		lTeamBase = mTeamBlack.GetBase();
		break;
	default:
		break;
	}
	return lTeamBase;
}
StatusTeam * StatusField::GetTeamAlly(CharacterBase * aMember)
{
	if (aMember == nullptr)return nullptr;


	for (decltype(auto)lMember : *mTeamBlack.GetMembers()) {
		if (lMember == aMember) {
			return &mTeamBlack;
		}
	}
	for (decltype(auto)lMember : *mTeamWhite.GetMembers()) {
		if (lMember == aMember) {
			return &mTeamWhite;
		}
	}
	return nullptr;
}
StatusTeam * StatusField::GetTeamEnemy(CharacterBase * aMember)
{
	if (aMember == nullptr)return nullptr;

	for (decltype(auto)lMember : *mTeamBlack.GetMembers()) {
		if (lMember == aMember) {
			return &mTeamWhite;
		}
	}
	for (decltype(auto)lMember : *mTeamWhite.GetMembers()) {
		if (lMember == aMember) {
			return &mTeamBlack;
		}
	}
	return nullptr;
}
eTeamType StatusField::GetTeamType(CharacterBase * aChara)
{
	for (decltype(auto)lMember : *mTeamBlack.GetMembers()) {
		if (lMember == aChara) {
			return eTeamType::Black;
		}
	}
	for (decltype(auto)lMember : *mTeamWhite.GetMembers()) {
		if (lMember == aChara) {
			return eTeamType::White;
		}
	}
	return{};//本来通らない
}
void StatusField::GetBall(CharacterBase * aChara)
{
	aChara->GetStatus()->mBall = mBall;
	SetBallHolder(aChara);
	DXVector3 lPosition;
	aChara->GetWorld()->GetMatrix()->GetT(lPosition);

	this->mEfkBallGet.SetPosition({ lPosition.x,lPosition.y,lPosition.z });
	this->mEfkBallGet.Play();

}
IssueFlag StatusField::IsWin(CharacterBase * aChara)
{
	auto lAlly = GetTeamAlly(aChara);
	auto lEnemy = GetTeamEnemy(aChara);

	IssueFlag lResult{ IssueFlag::Draw };
	if (lAlly->GetScore() > lEnemy->GetScore()) {
		lResult = IssueFlag::Win;
	}else if (lAlly->GetScore() < lEnemy->GetScore()) {
		lResult = IssueFlag::Lose;
	}

	return lResult;
}
DXVector3 StatusField::GetNodePosition(const int aIndex)
{
	DXVector3 lResult{};
	for (auto&lNode : mFieldNodes) {
		if (lNode->GetID() == aIndex) {
			lResult = static_cast<MyNode*>(lNode)->Position;
			break;
		}
	}
	return lResult;
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

void StatusField::GoalProccess(CharacterBase * aGoaler)
{
	RespawnBall();
	aGoaler->GetStatus()->mBall = nullptr;

	if (dynamic_cast<Enemy*>(aGoaler)) {
		aGoaler->GetStatus<EnemyStatus>()->mIsTargetingBall = false;
		
	}

	//ゴールしたチームに得点
	GetTeamAlly(aGoaler)->AddScore();

}

int StatusField::GetScoreWhite()
{
	return mTeamWhite.GetScore();
}

int StatusField::GetScoreBlack()
{
	return mTeamBlack.GetScore();
}

void StatusField::UpdateTime()
{
	auto lNowTime = std::chrono::system_clock::now();
	mRemainTime = mLimitTime - std::chrono::duration_cast<std::chrono::seconds>(lNowTime - mStartTime).count();
}

void StatusField::GameStart()
{
	mStartTime = std::chrono::system_clock::now();
}

bool StatusField::IsTimeOver()
{
	
	return mRemainTime <= 0;
}

void StatusField::GetRemainTime(int & aMinutes, int & aSeconds)
{
	aMinutes = mRemainTime / 60;
	aSeconds = mRemainTime % 60;
}

void StatusField::Update()
{
	mTeamWhite.GetBase()->UpdateMesh();
	mTeamBlack.GetBase()->UpdateMesh();

	mTeamWhite.GetBase()->GetWorld()->AddRC(0.0f, 1.0f, 0.0f);
	mTeamBlack.GetBase()->GetWorld()->AddRC(0.0f, 1.0f, 0.0f);

	mBall->GetWorld()->AddRC({ 1.5f,3.0f,4.5f });

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
