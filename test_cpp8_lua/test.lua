print( "test.lua" )
dofile( "pkg2_class.lua" )

function sleep (a) 
    local sec = tonumber(os.clock() + a)
    while (os.clock() < sec) do 
    end 
end

local nbs = NBSocket.Create()
print( nbs )

nbs:SetAddress( "127.0.0.1", 12345 )
nbs:Connect()
print( nbs )

local SS = NBSocketStates
while true do

	local state = nbs:GetState()
	if state == SS.Disconnected then
        goto LabEnd
	elseif state == SS.Connecting then
		-- 1 秒连不上就算超时吧
	    if nbs:GetTicks() > 10 then   
			nbs:Disconnect()
		end
	elseif state == SS.Connected then
	    -- 刚连上时发包
        if nbs:GetTicks() == 0 then
			local bb = BBuffer:Create()
			--bb:WriteByte( 4, 0, 49, 50, 51, 52, 4, 0, 53, 54, 55, 56 )
			local o = PKG2_基类.Create()
			o.不淋 = true -- Boolean
			o.白特 = 1 -- Byte
			o.撕白特 = -2 -- SByte
			o.吸哦特 = -3 -- Int16
			o.又吸哦特 = 4 -- UInt16
			o.硬特 = -5 -- Int32
			o.又硬特 = 6 -- UInt32
			o.浪 = -7 -- Int64
			o.又浪 = 8 -- UInt64
			o.扶裸特 = 9.1 -- Single
			o.大波 = 10.2 -- Double
			o.湿最硬 = "这是个串儿" -- String
			o.屄拔扶儿 = BBuffer.Create() -- BBuffer
			o.屄拔扶儿:WriteByte( 1 )
			o.屄拔扶儿:WriteByte( 2 )
			o.屄拔扶儿:WriteByte( 3 )
			bb:WritePackage( o )

			nbs:Send( bb )
		end
        -- dump 收到的包
        ::LabRetry::
        local buf = nbs:PopRecv()
        if buf ~= nil then
            print( buf )
            goto LabRetry
        end
		 -- 存活 10 帧后, 延迟 2 帧杀掉
        if nbs:GetTicks() > 10 then
			nbs:Disconnect( 2 ) 
		end
	elseif state == SS.Disconnecting then
		-- empty
	end

	-- 模拟帧循环 1秒10次
	print( nbs )
	sleep( 0.1 )
	nbs:Update()
end
::LabEnd::
print( nbs )
