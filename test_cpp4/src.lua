require "PKG_class"

local c = UvTcpClient.Create()

c:OnConnect(
	function(connected)
		if connected then
			print("OnConnect() Success")
			local pkg = WEB_WEB_testcpp3_Cmd2.Create()
			pkg.id = 12345
			local bb = BBuffer.Create()
			bb:WriteRoot(pkg)
			c:SendRequest(bb, function(o)
				print("Recv "..o.__proto.typeName..", errNum = "..o.errNum..", errMsg = "..o.errMsg)
				c:Disconnect()
			end)
		else
			print("Connect Fail")
		end
	end
)

c:OnDisconnect(
	function()
		print("OnDisconnect()")
	end
)

local isFirst = true
function Update()
	if isFirst and c:GetState() == 0 then
		c:ConnectEx("127.0.0.1", 11111)
		isFirst = false
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
