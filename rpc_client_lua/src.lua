function NBSocket_Create()
	local nbs_ = NBSocket.Create()
	local nbs = 
	{
		OnReceivePackage = function(ibb) end,			-- need set by user
		OnReceiveRequest = function(serial, ibb) end,	-- need set by user
		rpcDefaultInterval = 30 * 5,
		socket = nbs_,
		serial = 0,
	}
	nbs.__tostring = function(t)
		return nbs_:__tostring()
	end
	nbs.__index = function(t, k)
		if k == "state" then return nbs_:GetState() end
		if k == "ticks" then return nbs_:GetTicks() end
		if k == "alive" then return nbs_:GetState() == 2 end
	end
	setmetatable(nbs, nbs)
	nbs.SetAddress = function(ip, port)
		return nbs_:SetAddress(ip, port)
	end
	nbs.Connect = function()
		return nbs_:Connect()
	end
	nbs.Disconnect = function(delayTicks)
		return nbs_:Disconnect(delayTicks)
	end
	nbs.Send = function(pkg)
		return nbs_:Send(pkg)
	end
	nbs.PopRecv = function()
		return nbs_:PopRecv()
	end
	nbs.Update = function()
		local r = nbs_:Update()
		if r == 0 then
			while true do
				local bb = nbs_:PopRecv()
				if bb == nil then
					break
				end
				local pkgTypeId = bb:ReadByte()
				bb:SetOffset( bb:GetOffset() + 2 )
				local serial = 0
				if pkgTypeId == 1 or pkgTypeId == 2 then
					serial = bb:ReadUInt32()
				end
				local success, ibb = pcall(function() return bb:ReadRoot() end)
				if not success then
					nbs_:Disconnect()
					break
				end
				if pkgTypeId == 0 then
					nbs.OnReceivePackage(ibb)
				elseif pkgTypeId == 1 then
					nbs.OnReceiveRequest(serial, ibb)
				elseif pkgTypeId == 2 then
					nbs.Callback(serial, ibb)
				else
					nbs_:Disconnect()
					break;
				end
			end
		end

		-- rpc
		local ticks = nbs_:GetTicks()
		local timeout, serial, action
		while true do
			timeout, serial = nbs_:PeekSerial()
			if timeout == nil or timeout > ticks then
				return
			end
			action = nbs[serial]
			if action == nil then
				return
			end
			nbs[serial] = nil
			action(serial, nil)
			nbs_:PopSerial()
		end
		return r
	end
	nbs.SendRequest = function(pkg, cb, interval)
		if interval == nil then interval = nbs.rpcDefaultInterval end
		local serial = nbs.serial + 1
		nbs.serial = serial
		nbs[serial] = cb
		nbs_:PushSerial(nbs_:GetTicks() + interval, serial)
		local bb = BBuffer:Create()
		bb:WritePackage(pkg, 1, serial)
		local r = nbs_:Send( bb )
		return serial
	end
	nbs.Unregister = function(serial)
		nbs[serial] = nil
	end
	nbs.Callback = function(serial, ibb)
		local a = nbs[serial]
		if a == nil then return end
		nbs[serial] = nil
		a(serial, ibb)
	end
	return nbs
end

dofile("RPC_class.lua")

local yield = coroutine.yield
local resume = coroutine.resume

local nbs = NBSocket_Create()
local nbsco = coroutine.create(function()
	nbs.SetAddress("127.0.0.1", 12345)
	print("init")

	::LabConnect::
	yield()

	nbs.Connect()
	print("connecting...")
    while true do 
		yield()
		if nbs.state ~= NBSocketStates.Connecting then break end
        if nbs.ticks > 30 * 2 then break end
	end

	print("event occur");
	if nbs.state == NBSocketStates.Disconnected then
		local ticks = nbs.ticks + 30;
		while nbs.ticks < ticks do 
			yield()
		end
        print("can't connect to server");
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
    local serial = nbs.SendRequest(login, function(s, ibb)
		recv = ibb
	end)
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


	::LabPing::
    yield()

    print("request ping")
    while nbs.alive do
        yield()
        if nbs.ticks % 30 == 0 then
			local ping = RPC_Generic_Ping.Create()
			ping.ticks = os.clock() * 1000
            nbs.SendRequest(ping, function(s, ibb)
                if ibb == nil then
                    print("ping timeout")
                else
					local i64 = int64.new(os.clock() * 1000)
					i64 = i64 - ibb.ticks
                    print(i64)
                end
            end)
        end
    end
    print("disconnected")
    goto LabConnect

end)


while true do 
	resume(nbsco)			-- 驱动逻辑代码
	nbs.Update()			-- 驱动网络模块

	-- 模拟帧延迟
    local timeout = os.clock() + 0.033
    while (os.clock() < timeout) do end
end 

