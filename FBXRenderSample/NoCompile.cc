/*
	AI:
		1.	ボールを追う
		2.	ゴールに向かう(ボールを持っている)
		3.	テキを探す(無差別)
		4.	攻撃(敵発見時)
		5.	エネルギーチャージ
		6.	ターゲットを追いかける
		7.	ボールを持っているターゲットを追う
		8.	ボールを持っている味方を追う
		9.	味方から離れないようにターゲットを探す

	ダイクストラクラスに重みの係数を追加する
	初期値は1.0f
	ルート計算時にノード移動コストに、移動先ノードのこの係数をかけた上で計算する
	Value	:	float -> mWeightCost
*/



/*
	フラグ(AI)：
		[Attribute]	:	[Type]	:		[Name]			:	[Description]
		Field		:	bool	:	mBallHoldField	:	フィールドにボールが落ちているか
		Team		:	bool	:	mBallHoldAlly	:	味方がボールを持っているか
		Team		:	bool	:	mBallHoldEnemy	:	敵がボールを持っているか
		Character	:	bool	:	mBallHoldMe		:	自身が持っているか
		
		Character	:	bool	:	mInSightEnemy	:	敵が視界にいるか
		Character	:	bool	:	mLockonEnemy	:	敵を捕捉しているか
		Character	:	bool	:	mBallHoldTarget	:	捉えた敵がボールを持っているか
		
		Character	:	bool	:	mInSightAlly	:	味方が視界にいるか
		Character	:	bool	:	mAllyNear		:	味方付近にいるか
		Character	:	bool	:	mBallAllyNear	:	ボール所持味方の付近にいるか
		

		Character	:	bool	:	mChargedEnergy	:	エネルギーがあるか
		
	Compound Task
		自身がボールを持っているとき
		自身以外の味方がボールを持っているとき
		敵がボールを持っているとき
		フィールドにボールが落ちているとき
	Primitive Task
			ボールを拾いに行く
			ゴールに向かう
			エネルギーをチャージする
			エネルギーがなくなったので隠れる
			視界にいる敵を攻撃する
			ボールを持っている味方を追いかける
			ボールを持っている味方からあまり離れずに行動する
			無差別にターゲットを探す
			視界から消えた発見済みのターゲットを追う
			ボールを持っている敵を追いかける
			敵のゴール付近を探索する
			味方ののゴール付近を探索する


	//関数(AI)
		[Attribute]	:		[Name]			:	[Description]
		Compound	:	DecisionMake		:	AIエントリーポイント
		Compound	:	BallHoldingMe		:	自分がボールを持っているとき
		Compound	:	BallHoldingAlly		:	自分以外の味方がボールを持っているとき
		Compound	:	BallHoldingEnemy	:	敵がボールを持っているとき
		Compound	:	BallHoldingField	:	フィールドにボールが落ちているとき
		Primitive	:	MoveToBall			:	ボールを拾いに行く
		Primitive	:	MoveToGoal			:	ゴールへ行く
		Primitive	:	MoveToHide			:	チャージするために隠れる
		Primitive	:	ChargeEnergy		:	エネルギーをチャージする
		Primitive	:	InSightAttack		:	視界に入っている敵を攻撃する
		Primitive	:	MoveToBallHoldAlly	:	ボールを持っている味方の近くに移動する
		Primitive	:	SearchEnemyShort	:	味方からあまり離れずに敵を探す
		Primitive	:	SerachEnemyAll		:	無差別に敵を探す
		Primitive	:	MoveToLookingTarget	:	発見済みの敵を追いかける
		Primitive	:	MoveToBallTarget	:	ボールを持っている敵を追いかける
		Primitive	:	SearchForEnemyArea	:	敵エリアを探索する
		Primitive	:	SearchForAllyArea	:	味方エリアを探索する



*/

/*
	擬似AI

	Function{
		
	}

	Start{
		//自分ボールを持っているか
		if BallHoldMe == true then

		//味方がボールを持っているか
		elseif BallHoldAlly == true then

		//敵がボールを持っているか
		elseif BallHoldEnemy == true then

		//フィールドにボールが落ちているか
		elseif BallHoldField == true then
		end

		//味方がボールを持っているか
		if(BallHoldAlly==true){
			
		}
	}


*/
