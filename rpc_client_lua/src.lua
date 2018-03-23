require("netlib")
dofile("RPC_class.lua")

local yield = coroutine.yield
local resume = coroutine.resume

print("::LabBegin::")
::LabBegin::

local nbs = NBSocket_Create()
local nbsco = coroutine.create(function()
	print("init")

	::LabConnect::
	yield()

    nbs.Disconnect()
	nbs.Connect()
	nbs.SetAddress("127.0.0.1", 12345)
	print("connecting...")
    while true do 
		yield()
		if nbs.state ~= NBSocketStates.Connecting then break end
        if nbs.ticks > 30 * 2 then break end
	end

	print("event occur");
	if nbs.state == NBSocketStates.Disconnected then
        print("can't connect to server");
		local ticks = nbs.ticks + 30;
		while nbs.ticks < ticks do 
			yield()
		end
		goto LabConnect

	elseif nbs.state == NBSocketStates.Connecting then
		nbs.Disconnect()
        print("timeout")
        goto LabConnect
	end
    print("connect success");

	::LabLogin::
    yield()

    print("request login");
	local recv = 0
	local login = RPC_Client_Login_Login.Create()
	login.username = "a"
	login.password = "11111"
    local serial, r = nbs.SendRequest(login, function(s, ibb)
		recv = ibb
	end)
	if r < 0 then
        print("send error");
		local ticks = nbs.ticks + 30;
		while nbs.ticks < ticks do 
			yield()
		end
		goto LabConnect
	end
	print("wait login response");
    while recv == 0 do
		yield()
        if not nbs.alive then
            print("disconnected")
            nbs.Unregister(serial)

			local ticks = nbs.ticks + 30
            while nbs.ticks < ticks do yield() end
            goto LabConnect
        end
	end

	print("response callback")
    if recv == nil then
        if nbs.alive then
            print("timeout")
            goto LabLogin
        else
            print("disconnected")
            goto LabConnect
        end
    else
        if recv.__proto == RPC_Generic_Error then
            print("recv " .. recv.__proto.typeName .. " errMsg = " .. recv.errMsg)
            local ticks = nbs.ticks + 30
            while nbs.ticks < ticks do
                yield()
                if not nbs.alive then
					goto LabConnect
				end
            end
            goto LabLogin
        end
        print("login success")
	end


	nbs.Disconnect()	-- 模拟退出游戏时断开. 下面注释掉模拟游戏结束
	--[[
	-- 先注释掉. 

	::LabPing::
    yield()

    print("request ping")
    while nbs.alive do
        yield()
        if nbs.ticks % 30 == 0 then
			local ping = RPC_Generic_Ping.Create()
			ping.ticks = tostring(os.clock() * 1000)	-- int64 接受字串long值 或 低+高位uint32
            local serial, r = nbs.SendRequest(ping, function(s, ibb)
                if ibb == nil then
                    print("ping timeout")
                else
					local i64 = int64.new(os.clock() * 1000)
					i64 = i64 - ibb.ticks
                    print(i64)
                end
            end)
			if r < 0 then
				print("send error");
				local ticks = nbs.ticks + 30;
				while nbs.ticks < ticks do 
					yield()
				end
				goto LabConnect
			end
        end
    end
    print("disconnected")
	local ticks = nbs.ticks + 30;
	while nbs.ticks < ticks do 
		yield()
	end
	goto LabConnect
	]]
end)


while true do 
	resume(nbsco)			-- 驱动逻辑代码
	nbs.Update()			-- 驱动网络模块

	if coroutine.status(nbsco) == "dead" then		-- 模拟游戏结束后重进游戏
		print("goto LabBegin******************************************************")
		goto LabBegin
	end

	-- 模拟帧延迟
    local timeout = os.clock() + 0.033
    while (os.clock() < timeout) do end
end 

