--[[
local bb = BBuffer.Create()
bb:WriteByte( 1,2,3,4,5 )
print( bb )

]]

function GetTimeout(sec)
	return tonumber(os.clock() + sec)
end

local yield = coroutine.yield
local resume = coroutine.resume

local nbs = NBSocket.Create()
local nbsex = 
{
	serial = 0,
	mapping = {}	-- key: serial, value: { serial, cb }
}
function nbsex:Update()
	-- todo
end
function nbsex:SendRequest( pkg, cb )
	self.serial = self.serial + 1
	local bb = BBuffer:Create()
	bb:WritePackage( pkg, 1, serial )
	nbs:Send( bb )
end



local nbsco = coroutine.create(function()
	nbs:SetAddress("127.0.0.1", 12345)
	print("init")

	::LabConnect::
	yield()

	nbs:Connect()
	print("connecting...")
	local timeout = GetTimeout(2)
    while true do 
		yield()

		if nbs:GetState() ~= NBSocketStates.Connecting then break end
        if os.clock() > timeout then break end
	end
	print("event occur");
	if nbs:GetState() == NBSocketStates.Disconnected then
		timeout = GetTimeout(1)
		while os.clock() < timeout do 
			yield()
		end
        print("can't connect to server");
		goto LabConnect

	elseif nbs:GetState() == NBSocketStates.Connecting then
		nbs:Disconnect()
        print("timeout")
        goto LabConnect
	end
    print("connect success");

	::LabLogin::
    yield()

    print("request login");
	local recv = nil
	--[[
    var serial = nbs.SendRequest(           // 发登录请求
        new RPC.Client_Login.Login          // 构造一个登录包( 也可以用个静态的临时改值 )
        {
            username = "a",                 // 模拟读取用户输入
            password = (nbs.ticks % 10 > 0) ? "asdf" : "11111"     // 一定机率输对密码
        }
        , (s, ibb) => { recv = ibb; });     // RPC 回调: 将结果存入上下文变量以便后续 while 中判断

    Console.WriteLine("wait login response");
    while (recv == BBuffer.instance)        // 如果回调没发生就一直等
    {
        yield return null;
        if (!nbs.alive)                     // 等待期间断线
        {
            Console.WriteLine("disconnect");
            nbs.Unregister(serial);         // 反注册事件, 免得后续干扰( 除非每次对比 serial )
            goto LabConnect;
        }
    }
	]]

end)


while true do 
	resume(nbsco)			-- 驱动逻辑代码
	nbs:Update()			-- 驱动网络模块

	-- 模拟帧延迟
    local timeout = GetTimeout(0.033)
    while (os.clock() < timeout) do end
end 
