--[[
  HTN Planner tutorial code for lecture at CEDEC2016

  Copyright (c) 2016, Yohei Hase. All rights reserved.
  This source code is licensed under the MIT license.
]]

package.path=package.path..";tracing/?.lua"
require("htn")



domain = { primitive={}, compound={} }
--------------------------------------------------
-- primitive Task

--ボールを追っかける
domain.primitive.MoveToBall = function(state)
	--移動完了後
	--ボールを所持状態とする
	if state.mBallHoldMe == false then
		state.mBallHoldMe = true
		return true;
	end
	return false;
end

--ゴールへ移動する
domain.primitive.MoveToGoal = function(state)
	--ボールをもって言った場合はボールリセット
	if state.mBallHoldMe == true then
		state.mBallHoldMe = false 
		return true
	end
	return false
end

--隠れる
domain.primitive.MoveToHide = function(state)
	--敵を視認している状態だった場合、
	--ロックオン状態にして、視界から外す
	if state.mInSightEnemy == true then
		state.mInSightEnemy = false
		state.mLockonEnemy = true
		return true
	end
	return false
end

--エネルギーチャージ
domain.primitive.ChargeEnergy = function(state)
	--チャージされてない場合はチャージする
	if state.mChargedEnergy == false then
		state.mChargedEnergy = true
		return true
	end
	return false
end

--視界に捉えている敵を攻撃する
domain.primitive.InSightAttack = function(state)
	if state.mInSightEnemy == true and state.mChargedEnergy == true then
		state.mInSightEnemy=false
		state.mLockonEnemy=false
		return true
	end
	return false
end

--ボールを持っている味方を追いかける
domain.primitive.MoveToBallHoldAlly = function(state)
	if state.mBallAllyNear == false then
		state.mBallAllyNear = true
		return true
	end
	return false
end

--味方からあまり離れずに敵を探す
domain.primitive.SearchEnemyShort = function(state)
	if state.mInSightEnemy == false then
		state.mInSightEnemy = true
		return true
	end
	return false
end

--心を無にして何も考えず敵を探す
domain.primitive.SearchEnemyAll = function(state)
--敵を発見する
	if state.mInSightEnemy == false then
		state.mInSightEnemy = true
		return true
	end
	return false
end

--敵を捕捉済みの場合の場合、追いかける
domain.primitive.MoveToLookingTarget = function(state)
	if state.mLockonEnemy == true then
		state.mInSightEnemy = true
		return true
	end
	return false
end

--ボールを持っている敵を追いかける
domain.primitive.MoveToBallTarget = function(state)
	if state.mInSightEnemy == false then
	--敵を発見する
		state.mInSightEnemy = true
		return true
	end
	return false
end

--敵エリアを探索する
domain.primitive.SearchForEnemyArea = function(state)
	if state.mInSightEnemy == false then
	--敵を発見する
		state.mInSightEnemy = true
		return true
	end
	return false
end

--味方エリアを探索する
domain.primitive.SearchForAllyArea = function(state)
	--敵を発見する
	if state.mInSightEnemy == false then
		state.mInSightEnemy = true
		return true
	end
	return false
end

--------------------------------------------------
-- compound Task
domain.compound.AttackPrepare = {
	function(state)
		--敵が視界にいた場合、隠れる
		if state.mChargedEnergy == false then
			if state.mInSightEnemy == true then
				return {{"MoveToHide"},{"AttackPrepare"}}
			else
				return {{"ChargeEnergy"},{"AttackPrepare"}}
			end
		end		
		return {}
	end
}
--自分がボールを持っているとき
--[[
	優先順位：
		1.	エネルギーチャージ
		2.	ゴールへ移動
		3.	敵の追尾
		4.	攻撃
]]
domain.compound.BallHoldingMe={


	function(state)

	--ターゲットを捕捉済みで追跡する場合
		if state.mLockonEnemy == true and state.mInSightEnemy == false then
			return {{"MoveToLookingTarget"},{"BallHoldingMe"}}
		--敵がいれば攻撃
		elseif state.mInSightEnemy == true then
			return {{"InSightAttack"}}
		end
		return {}
	end
}


--自分以外の味方がボールを持っているとき
--[[
	優先順位：
		1.	エネルギーチャージ
		2.	ボール所持者の追跡
		3.	付近の敵の探索
		4.	敵エリアの探索
]]
domain.compound.BallHoldingAlly={
	function(state)
		--ターゲットを捕捉済みの時
		if state.mLockonEnemy == true and state.mInSightEnemy == false then
			--味方と離れすぎてる場合は味方を追いかける
			if state.mBallAllyNear == false then
				return {{"MoveToBallHoldAlly"},{"BallHoldingAlly"}}
			else
				return {{"MoveToLookingTarget"},{"BallHoldingAlly"}}
			end
		--攻撃できる状態の時
		elseif state.mInSightEnemy == true then
			return {{"InSightAttack"},{"BallHoldingAlly"}}
		--ボール所持者と離れているとき追跡
		elseif state.mBallAllyNear == false then
			return {{"MoveToBallHoldAlly"},{"BallHoldingAlly"}}
		--ボール所持味方の近くまで来たとき
		elseif state.mBallAllyNear == true then
			--味方付近を探索する
			return {{"SearchForAllyArea"}}
		end
		return {}	
	end
}


--敵がボールを持っているとき
--[[
	優先順位：
		1.	エネルギーチャージ
		2.	敵エリアの探索
		3.	敵ゴールへ行く
]]
domain.compound.BallHoldingEnemy={
	function(state)
		--ターゲットを捕捉済みの時
		if state.mLockonEnemy == true and state.mInSightEnemy == false then
			return {{"MoveToLookingTarget"},{"BallHoldingEnemy"}}
		elseif state.mInSightEnemy == true then
			--ターゲットがボール所持していた場合
			return {{"InSightAttack"}}
		--ターゲットを探す
		elseif state.mInSightEnemy == false then
			return {{"MoveToBallTarget"},{"BallHoldingEnemy"}}
		end
		return {}
	end
}

--フィールドにボールが落ちているとき
--[[
	優先順位：
		1.	エネルギーチャージ
		2.	ボールを拾いに行く
		3.	敵エリアの探索
]]
domain.compound.BallHoldingField={
	function(state)
				--ターゲットを捕捉済みの時
		if state.mLockonEnemy == true and state.mInSightEnemy == false then
			return {{"MoveToLookingTarget"},{"BallHoldingField"}}
		elseif state.mInSightEnemy == true then
			return {{"InSightAttack"},{"BallHoldingField"}}
		end
		return {}
	end
}
--AIエントリポイント
domain.compound.DescisionMake={
	function(state)
		--自身がボールを持っている場合
		if state.mBallHoldMe == true then
			return {{"AttackPrepare"},{"BallHoldingMe"},{"MoveToGoal"}}
		--自分以外の味方がボールを持っていた場合
		elseif state.mBallHoldAlly == true then
			return {{"AttackPrepare"},{"BallHoldingAlly"}}
		--敵がボールを持っていた場合
		elseif state.mBallHoldEnemy == true then
			return {{"AttackPrepare"},{"BallHoldingEnemy"},{"MoveToBall"}}
		--フィールドにボールが落ちている場合
		elseif state.mBallHoldField == true then
			return {{"AttackPrepare"},{"BallHoldingField"},{"MoveToBall"}}
		end
	end
}
--------------------------------------------------
-- state

--state.bLockoned=false		--敵を捕捉済みか？

function GetPlan(
	aBallHoldField,
	aBallHoldAlly,
	aBallHoldEnemy,
	aBallHoldMe,

	aInSightEnemy,
	aLockonEnemy,
	aBallHoldTarget,

	aInSightAlly,
	aBallAllyNear,
	aAllyNear,

	aChargedEnergy
)
	state={}

	--BallHolder--
	state.mBallHoldField	=	aBallHoldField	--フィールドにボールが落ちているか
	state.mBallHoldAlly		=	aBallHoldAlly	--自分以外の味方がボールを持っているか
	state.mBallHoldEnemy	=	aBallHoldEnemy	--敵がボールを持っているか
	state.mBallHoldMe		=	aBallHoldMe	--ボールを所持しているか

	--EnemyState--
	state.mInSightEnemy		=	aInSightEnemy	--敵が視界にいるか
	state.mLockonEnemy		=	aLockonEnemy	--敵を捕捉済みか
	state.mBallHoldTarget	=	aBallHoldTarget	--捉えた敵がボールを持っているか

	--AllyState--
	state.mInSightAlly	=	aInSightAlly	--味方が視界にいるか
	state.mBallAllyNear	=	aBallAllyNear	--ボールを持っている味方の近くにいるか
	state.mAllyNear		=	aAllyNear	--味方の近くにいるか

	--Other
	state.mChargedEnergy=	aChargedEnergy	--エネルギーチャージ

	print("*** Start ***")
	plan = htn(domain, state, {{"DescisionMake"}})
	print_plan(plan)
	print("***  End  ***")
	return GetPlanArray(plan)
end
