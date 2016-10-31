--[[
  HTN Planner tutorial code for lecture at CEDEC2016

  Copyright (c) 2016, Yohei Hase. All rights reserved.
  This source code is licensed under the MIT license.
]]

package.path=package.path..";tracing/?.lua"
require("htn")

--defined
--[[
	実装部分：
	
		攻撃(attack):
			遠距離攻撃：
				攻撃準備(attackPrepare) > energyShot

		攻撃準備(attackPrepare)：
			エネルギーは溜まっているか(IsEnergy):false
				敵が視界にいるか(IsTargeting):true
					視界外に移動する movingHide > chargeEnergy > attackPrepare
				:false
					チャージ chargeEnergy > attackPrepare
			:true
				敵が視界にいるか(IsTargeting):true
					準備オーケー {}
				:false
					ターゲットを探す searchTarget > attackPrepare


]]
--[[
	state:
		bTargeting		--ターゲットを捉えているか
		chargedEnergy	--エネルギーがチャージされているか

--utility:
	IsEnergy
	IsTargeting

--primitive:
	energyShot
	movingHide
	chargeEnergy
	searchTarget

--compound:
	attack
	attackPrepare


]]


domain = { primitive={}, compound={} }
--------------------------------------------------
-- primitive Task
--攻撃
domain.primitive.energyShot = function(state)
	--エネルギーが溜まっていてなおかつ敵を捉えていたら攻撃
	if state.bTargeting == true and state.chargedEnergy ==true then
		return true
	end
	return false
end

--物陰に隠れる
domain.primitive.movingHide = function(state)
	--ターゲットが視界にいた場合
	if state.bTargeting == true then
		state.bTargeting=false
		state.bLockoned=true
		return true
	end
	return false
end

--エネルギーのチャージ
domain.primitive.chargeEnergy = function(state)
	--敵が視界にいなければチャージ
	if state.chargedEnergy == false and state.bTargeting ==false then
		state.chargedEnergy = true
		return true
	end
	return false
end

--ターゲットを探す
domain.primitive.searchTarget = function(state)
	--敵を探す
	if state.bTargeting == false then
		state.bTargeting=true
		return true
	end
	return false
end

--ターゲットを追う
domain.primitive.moveToTarget = function(state)
	--敵を認識済みであれば追う
	if state.bLockoned == true then
		state.bLockoned=false
		state.bTargeting=true
		return true
	end
	return false
end

domain.primitive.moveToGoal = function(state)
	--ゴールに向かう
	if state.bHoldBall == true and state.bTargeting == false then
		state.bHoldBall=false
		return true
	end
	return false
end

domain.primitive.moveToBall = function(state)
	--ボールを拾いに行く
	if state.bBallIsField == true then
		state.bBallIsField = false
		state.bHoldBall =true
		return true
	end
	return false
end

domain.primitive.moveToBallTarget = function(state)
	--ボールを持っているターゲットを追いかける
	if state.bBallIsField == false and state.bTargetHoldBall == true then
		state.bLockoned = true
		state.bTargeting=true
		return true
	end
	return false
end

--------------------------------------------------
-- compound Task

--攻撃
domain.compound.attack={
	function(state)
		return {{"attackPrepare"}}
	end
}

--攻撃準備
domain.compound.attackPrepare={
	function(state)
		--エネルギーがなければチャージ
		if state.chargedEnergy == false then
			if state.bTargeting ==true then
			--視界に敵がいた場合、物陰に隠れる
				return {{"movingHide"},{"attackPrepare"}}
			else
			--視界に敵がいなかった場合、チャージ
				return {{"chargeEnergy"},{"attackPrepare"}}
			end
		--一度物陰に隠れてチャージした場合、隠れる前の位置に移動する
		elseif state.bLockoned == true  then
			return {{"moveToTarget"},{"energyShot"}}	
		--チャージ後(もしくはチャージ済みでボールを持っていて敵が視界からいない場合)
		elseif state.bHoldBall == true and state.bTargeting == false then
			return {{"moveToGoal"},{"attackPrepare"}}
		--ボールが落ちていて、それを追いかける場合
		elseif state.bBallIsField == true and state.bTargeting == false then
			return {{"moveToBall"},{"attackPrepare"}}
		--ボールが落ちていない(誰かがボールを持っている場合)
		elseif state.bBallIsField == false and state.bTargetHoldBall == true then
			return {{"moveToBallTarget"},{"energyShot"}}
		elseif state.bTargeting == true and state.bHoldBall == true then
			return {{"energyShot"}}
		--チャージ済みで視界に敵がいなかった場合、敵を探す
		elseif state.bTargeting == false then
			return {{"searchTarget"},{"attackPrepare"}}
		end
		--ここまでくれば攻撃準備完了
		return {}
	end	
}

--------------------------------------------------
-- state

--state.bLockoned=false		--敵を捕捉済みか？

function GetPlan(aTargeting,aTarget,aChargedEnergy,aTargetHoldBall,aHoldingBall,aBallIsField)


	state={}
	state.bTargeting=aTargeting				--敵が視界にいるかどうか？
	state.bLockoned=aTarget					--敵を捉えていたかどうか
	state.chargedEnergy=aChargedEnergy		--エネルギーがチャージ済みか？
	state.bTargetHoldBall=aTargetHoldBall	--ターゲットがボールを持っているかどうか
	state.bHoldBall=aHoldingBall			--ボールを所持しているかどうか
	state.bBallIsField=aBallIsField			--ボールがフィールドに落ちているかどうか
	plan = htn(domain, state, {{"attack"}})
	print("EndEnemy")
	print_plan(plan)
	return GetPlanArray(plan)
end
GetPlan(false,false,true,true,false,false)
