#pragma once
#include"Dijkstra.h"
#include"../DXVector3.h"
class NcgLuaManager;
struct StatusField;
struct MyNode :public Dijkstra::Node
{
	std::string mName = "Empty";
	DXVector3 Position;
	MyNode(int id, const std::string& name,const DXVector3&position) :Node(id) {
		mName = name;
		Position = { position };
	}

};

namespace EnemyAIType {
	enum Type {
		//eEnergyShot,
		//eMovingHide,
		//eChargeEnergy,
		//eSearchTarget,
		//eMoveToTarget,
		//eMoveToGoal,
		//eMoveToBall,
		//eMoveToBallTarget
		eUnKnown,
		eMoveToBall,
		eMoveToGoal,
		eMoveToHide,
		eChargeEnergy,
		eInSightAttack,
		eMoveToBallHoldAlly,
		eSearchEnemyShort,
		eSerachEnemyAll,
		eMoveToLookingTarget,
		eMoveToBallTarget,
		eSearchForEnemyArea,
		eSearchForAllyArea
	};


	const std::string sUnKnown = "unKnown";
	const std::string sMoveToBall = "MoveToBall";
	const std::string sMoveToGoal = "MoveToGoal";
	const std::string sMoveToHide = "MoveToHide";
	const std::string sChargeEnergy = "ChargeEnergy";
	const std::string sInSightAttack = "InSightAttack";
	const std::string sMoveToBallHoldAlly = "MoveToBallHoldAlly";
	const std::string sSearchEnemyShort = "SearchEnemyShort";
	const std::string sSerachEnemyAll = "SerachEnemyAll";
	const std::string sMoveToLookingTarget = "MoveToLookingTarget";
	const std::string sMoveToBallTarget = "MoveToBallTarget";
	const std::string sSearchForEnemyArea = "SearchForEnemyArea";
	const std::string sSearchForAllyArea = "SearchForAllyArea";



	//const std::string sEnergyShot = "energyShot";
	//const std::string sMovingHide = "movingHide";
	//const std::string sChargeEnergy = "chargeEnergy";
	//const std::string sSearchTarget = "searchTarget";
	//const std::string sMoveToTarget = "moveToTarget";
	//const std::string sMoveToGoal = "moveToGoal";
	//const std::string sMoveToBall = "moveToBall";
	//const std::string sMoveToBallTarget = "moveToBallTarget";


	Type ConvertString(const std::string&aStr);
}




class EnemyAI :public Dijkstra::Manager {
public:

	virtual ~EnemyAI();

	virtual void CreateNodes(StatusField&aField)override;
	//近い順にノードを取得する
	//第一引数、中心となる座標
	std::vector<MyNode*> GetNearNodeList(const DXVector3&pCenter);
	//AIの更新
	void Update(
		bool aBallHoldField,
		bool aBallHoldAlly,
		bool aBallHoldEnemy,
		bool aBallHoldMe,

		//EnemyState
		bool aInSigntEnemy,
		bool aLockonEnemy,
		bool aBallHoldTarget,

		//AllyState
		bool aInSightAlly,
		bool aBallAllyNear,
		bool aAllyNear,

		//Other
		bool aChargedEnergy
	);
	//現在の行動パターンを取得する
	EnemyAIType::Type GetNowAI();
	//AIを一つすすめる
	//すすめることができなかった場合はfalseが返される
	bool NextAI();
	//無条件にAIを停止させる
	void ClearAI();

	//AI用
	NcgLuaManager* mLuaAI;
	std::vector<EnemyAIType::Type>mAIType;
};
