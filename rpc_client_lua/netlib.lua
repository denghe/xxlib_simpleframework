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
		if delayTicks == nil then delayTicks = 0 end
		return nbs_:Disconnect(delayTicks)
	end
	local bbSend = BBuffer.Create()
	nbs.Send = function(pkg)
		bbSend:Clear()
		bbSend:WritePackage(pkg)
		return nbs_:Send(bbSend)
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
		bbSend:Clear()
		bbSend:WritePackage(pkg, 1, serial)
		local r = nbs_:Send( bbSend )
		return serial, r
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

