require "PKG_class"

local c = UvTcpClient.Create()

c:OnConnect(
	function(connected)
		if connected then
			print("连接成功")
			local pkg = WEB_WEB_testcpp3_Cmd2.Create()
			pkg.id = 12345
			local bb = BBuffer.Create()
			bb:WriteRoot(pkg)
			c:SendRequest(bb, function(o)
				if o == nil then
					return
				end
				print("收到包 类型为 "..o.__proto.typeName..", errNum = "..o.errNum..", errMsg = "..o.errMsg)
				c:Disconnect(false)
				print("主动断开")
			end)
		else
			print("连接失败")
		end
	end
)

c:OnDisconnect(
	function()
		print("被动断开")
	end
)

local frameNumber = 0
function Update()
	--if c:GetState() == 0 then
	frameNumber = frameNumber + 1
	if frameNumber == 1 then
		c:Disconnect(false)
		local r = c:ConnectEx("192.168.1.254", 11111, 2000)
		print("ConnectEx 返回值 = "..r)
	end

	if frameNumber == 10 then
		c:Disconnect(false)
		local r = c:ConnectEx("192.168.1.254", 11111, 2000)
		print("ConnectEx 返回值 = "..r)
	end
end











function UpdateCore()
	local typeId, func, bb, serial = c:PopMessage()
	if typeId == -3 then
		func()
	elseif typeId == -2 then
		func(false)
	elseif typeId == -1 then
		func(nil)
	elseif typeId == 0 then
		local success, pkg = pcall(function() return bb:ReadRoot() end)
		if success then
			func(pkg)
		else
			func(nil)
		end
	elseif typeId == 1 then
		local success, pkg = pcall(function() return bb:ReadRoot() end)
		if success then
			func(serial, pkg)
		else
			func(serial, nil)
		end
	elseif typeId == 2 then
		local success, pkg = pcall(function() return bb:ReadRoot() end)
		if success then
			func(pkg)
		else
			func(nil)
		end
	elseif typeId == 3 then
		func(true)
	end
end

function Sleep(n)
   local t0 = os.clock()
   while os.clock() - t0 <= n do end
end

for i = 1, 999999 do
	Sleep(0.5)
	UvLoop.Run()
	UpdateCore()
	Update()
	print(c)
end
