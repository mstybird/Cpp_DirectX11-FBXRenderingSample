require("tracing\\main")

--タスクの初期化
domain = {primitive={},compound={}}

--ユーティリティ


--出発地から目的地までの距離(重み付け)
function getDistance(a, b)
	local dist = { home={}, tokyo={}, haneda_airport={}, fukuoka={} }
	dist["home"]["tokyo"] = 1;
	dist["home"]["haneda_airport"] = 21;
	dist["home"]["fukuoka_airport"] = 881;
	dist["home"]["fukuoka"] = 901;
	dist["tokyo"]["haneda_airport"] = 20;
	dist["tokyo"]["fukuoka_airport"] = 880;
	dist["tokyo"]["fukuoka"] = 900;
	dist["haneda_airport"]["fukuoka_airport"] = 860;
	dist["haneda_airport"]["fukuoka"] = 880;
	dist["fukuoka"]["fukuoka_airport"] = 20;
	
	return (dist[a] and dist[a][b]) or (dist[b] and dist[b][a])
end

function getTrainPrice(a, b)
	local price = { tokyo={}, fukuoka={} }
	price["tokyo"]["haneda_airport"] = 200;
	price["tokyo"]["fukuoka"] = 30000
	price["fukuoka"]["fukuoka_airport"] = 200;
	
	return (price[a] and price[a][b]) or (price[b] and price[b][a])
end

function getAirPrice(a, b)
	local price = { haneda_airport={} }
	price["haneda_airport"]["fukuoka_airport"] = 50000;
	
	return (price[a] and price[a][b]) or (price[b] and price[b][a])
end

function getStation(a)
	local station = { home="tokyo", tokyo="tokyo", haneda_airport="haneda_airport", fukuoka="fukuoka", fukuoka_airport="fukuoka_airport" }
	return station[a]
end

function getAirport(a)
	local airport = { home="haneda_airport", tokyo="haneda_airport", haneda_airport="haneda_airport", fukuoka="fukuoka_airport", fukuoka_airport="fukuoka_airport" }
	return airport[a]
end

--プリミティブタスク

--歩く
domain.primitive.walk=function(state,a)
	--目的地に直接移動
	state.at=a
	return true
end

--切符を買う
--state	:現在の状態
--a		:出発駅
--b		:到着駅
domain.primitive.buyTrainTicket=function(state,a,b)
	--チケットの値段
	local price=getTrainPrice(a,b)
	--所持金がチケット代以上持っていれば購入。所持金を減らす
	if state.money>=price then
		state.money=state.money-price
		return true
	end
	return false
end


--電車に乗る
domain.primitive.rideTrain=function(state,a)
	--目的地まで移動する
	state.at=a
	return true
end

--航空券を買う
--切符が航空券に変わっただけでやってることは同じ
domain.primitive.buyAirTicket=function(state,a,b)
	local price=getAirPrice(a,b)
	if state.money>=price then
		state.money=state.money-price
		return true
	end
	return false
end

--飛行機に乗る
domain.primitive.rideAirplane=function(state,a)
	--目的地まで移動する
	state.at=a
	return true
end

--抽象タスク
domain.compound.move={
	function(state,to)
		local from=state.at
		--目的地に到着した
		if from==to then
			return {}
		end
		
		--目的地までの距離を計算
		local dist=getDistance(from,to)
		
		--飛行機で行く場合
		if dist>300 then
			local airA=getAirport(from)
			local airB=getAirport(to)
			local airPrice=getAirPrice(airA,airB)
			if airPrice ~= nil and airPrice<=state.money then
				return {{"buyAirTicket", airA, airB}, {"move", airA}, {"rideAirplane", airB}, {"move",to}}
			end
		end
		-- 電車
		if dist > 2 then
			local stationA = getStation(from)
			local stationB = getStation(to)
			local trainPrice = getTrainPrice(stationA, stationB)
			if trainPrice ~= nil and trainPrice <= state.money then
				return {{"move", stationA}, {"buyTrainTicket", stationA, stationB}, {"rideTrain", stationB}, {"move", to}}
			end
		end
		-- 徒歩
		return {{"walk", to}}
	end
}

state = {}
state.at = "home"
state.money = 100000
--state.money = 100

plan = htn(domain, state, {{"move", "fukuoka"}})
print_plan(plan)

















