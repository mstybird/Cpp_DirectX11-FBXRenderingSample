#pragma once
#include"Dijkstra.h"
#include"../DXVector3.h"
class NcgLuaManager;
struct MyNode :public Dijkstra::Node
{
	string mName = "Empty";
	DXVector3 Position;
	MyNode(int id, const string& name,const DXVector3&position) :Node(id) {
		mName = name;
		Position = { position };
	}

};

namespace EnemyAIType {
	enum Type {
		eUnKnown,
		eEnergyShot,
		eMovingHide,
		eChargeEnergy,
		eSearchTarget,
		eMoveToTarget
	};
	const std::string sUnKnown = "unKnown";
	const std::string sEnergyShot = "energyShot";
	const std::string sMovingHide = "movingHide";
	const std::string sChargeEnergy = "chargeEnergy";
	const std::string sSearchTarget = "searchTarget";
	const std::string sMoveToTarget = "moveToTarget";


	Type ConvertString(const std::string&aStr);
}




class EnemyAI :public Dijkstra::Manager {
public:
	virtual void CreateNodes();
	//近い順にノードを取得する
	//第一引数、中心となる座標
	std::vector<MyNode*> GetNearNodeList(const DXVector3&pCenter);
	//AIの更新
	void Update(bool aTargeting,bool aTarget, bool aChargedEnergy);
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
