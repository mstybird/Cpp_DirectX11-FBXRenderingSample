--tableのコピー
--ただの代入だと参照をコピーするだけ
--ただunpackするだけだと、tableの中身がtableだった場合、
--そのtableは参照のコピーとして処理されてしまうので、
--tableだったら展開してコピーという処理を取る
function deepcopy(a)
    if type(a)=="table" then
        local copy={}
        for key,var in next,a,nil do
            copy[key]=deepcopy(var)
        end
        return copy
    else
        return a
    end
end

--tableの連結
--aの後ろにbを連結する
function array_merge(a,b)
    local newArray={table.unpack(a)}
    for i,var in ipairs(b) do
        table.insert(newArray,var)
    end
    return newArray
end

--プランの出力
function print_plan(plan)
    if plan == false or plan == nil then
        print("not found")
    else
        for i,var in ipairs(plan) do
            print(i .. ": " .. table.concat(var,", "))
        end
    end
end

--domain:タスク
--state:プラン実行前の状態
--tasks:タスク
function htn(domain,state,tasks)
    return htn_internal(domain,state,tasks,{})
end

function htn_internal(domain,state,tasks,plan)
    --次のタスクがなければプラン確定
    if next(tasks)==nil then
        return plan
    end
    --タスクがあれば処理する
    --先頭のタスク(実行するタスク)を一つ
    local task=table.remove(tasks,1)
    local taskName=table.remove(task,1)
    --プリミティブタスクがあった場合
    --分割不可のタスク
    if domain.primitive[taskName]~=nil then
        --現在の状態の実体をコピー
        local newState=deepcopy(state)
        --プリミティブタスクの実行
        local res=domain.primitive[taskName](newState,table.unpack(task))
        --実行した結果がtrueであれば、そのタスクを深堀せずに、
        --次のタスクを実行する
        if res == true then
            return htn_internal(domain,newState,tasks,array_merge(plan,{{taskName,table.unpack(task)}}))
        else
            return false
        end
        
    --分割可能な抽象タスク
    elseif domain.compound[taskName]~=nil then
    --一つの抽象タスクに含まれている関数の個数分
    --ループする
    for i,func in ipairs(domain.compound[taskName]) do
            --それぞれの抽象タスクを実行
            local res=func(state,table.unpack(task))
            
            --結果が偽じゃなければプラン生成の続行
            if res ~= false then
                --先ほどの抽象タスクを実行した結果をtableに追加して
                --HTNの再起
                res = htn_internal(domain,state,array_merge(res,tasks),plan)
                --その結果がfalseではない(空ではない)場合は、
                --そのプランを返す
                if res ~= false then
                    return res
                end
            end
        end
    end
    return false
end



