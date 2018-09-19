
RPC_PkgGenMd5_Value = 'dfd6327ef3db0b69dc7ca345d134ba72'

--[[
服务类型列表
]]
RPC_Generic_ServiceTypes = {
    --[[
    登录
    ]]
    Login = 0,
    --[[
    数据库
    ]]
    DB = 1,
    --[[
    管理端
    ]]
    Manage = 2
}
RPC_DB_Manage_MsgResult = {
    typeName = "RPC_DB_Manage_MsgResult",
    typeId = 11,
    Create = function()
        local o = {}
        o.__proto = RPC_DB_Manage_MsgResult
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.txt = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.txt = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.txt )
    end
}
BBuffer.Register( RPC_DB_Manage_MsgResult )
RPC_Manage_DB_Msg = {
    typeName = "RPC_Manage_DB_Msg",
    typeId = 12,
    Create = function()
        local o = {}
        o.__proto = RPC_Manage_DB_Msg
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.txt = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.txt = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.txt )
    end
}
BBuffer.Register( RPC_Manage_DB_Msg )
RPC_Login_Client_LoginSuccess = {
    typeName = "RPC_Login_Client_LoginSuccess",
    typeId = 4,
    Create = function()
        local o = {}
        o.__proto = RPC_Login_Client_LoginSuccess
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.id = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.id )
    end
}
BBuffer.Register( RPC_Login_Client_LoginSuccess )
RPC_Client_Login_Login = {
    typeName = "RPC_Client_Login_Login",
    typeId = 5,
    Create = function()
        local o = {}
        o.__proto = RPC_Client_Login_Login
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.username = null -- String
        o.password = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.username = ReadObject( bb )
        o.password = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.username )
        WriteObject( bb, o.password )
    end
}
BBuffer.Register( RPC_Client_Login_Login )
RPC_DB_Login_AuthSuccess = {
    typeName = "RPC_DB_Login_AuthSuccess",
    typeId = 6,
    Create = function()
        local o = {}
        o.__proto = RPC_DB_Login_AuthSuccess
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.id = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.id )
    end
}
BBuffer.Register( RPC_DB_Login_AuthSuccess )
RPC_Login_DB_Auth = {
    typeName = "RPC_Login_DB_Auth",
    typeId = 7,
    Create = function()
        local o = {}
        o.__proto = RPC_Login_DB_Auth
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.username = null -- String
        o.password = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.username = ReadObject( bb )
        o.password = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.username )
        WriteObject( bb, o.password )
    end
}
BBuffer.Register( RPC_Login_DB_Auth )
--[[
服务间创建连接, 刚连上时, 首包需要发这个来说明自己是谁
]]
RPC_Generic_ServiceInfo = {
    typeName = "RPC_Generic_ServiceInfo",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = RPC_Generic_ServiceInfo
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        --[[
        服务类型
        ]]
        o.type = 0 -- RPC_Generic_ServiceTypes
        return o
    end,
    FromBBuffer = function( bb, o )
        o.type = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.type )
    end
}
BBuffer.Register( RPC_Generic_ServiceInfo )
--[[
通用错误返回
]]
RPC_Generic_Error = {
    typeName = "RPC_Generic_Error",
    typeId = 8,
    Create = function()
        local o = {}
        o.__proto = RPC_Generic_Error
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        --[[
        错误码
        ]]
        o.errNo = 0 -- Int32
        --[[
        错误文字
        ]]
        o.errMsg = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.errNo = bb:ReadInt32()
        o.errMsg = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.errNo )
        bb:WriteObject( o.errMsg )
    end
}
BBuffer.Register( RPC_Generic_Error )
--[[
心跳保持兼延迟测试 -- 请求
]]
RPC_Generic_Ping = {
    typeName = "RPC_Generic_Ping",
    typeId = 9,
    Create = function()
        local o = {}
        o.__proto = RPC_Generic_Ping
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.ticks = 0 -- Double
        return o
    end,
    FromBBuffer = function( bb, o )
        o.ticks = bb:ReadDouble()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteDouble( o.ticks )
    end
}
BBuffer.Register( RPC_Generic_Ping )
--[[
心跳保持兼延迟测试 -- 回应
]]
RPC_Generic_Pong = {
    typeName = "RPC_Generic_Pong",
    typeId = 10,
    Create = function()
        local o = {}
        o.__proto = RPC_Generic_Pong
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.ticks = 0 -- Double
        return o
    end,
    FromBBuffer = function( bb, o )
        o.ticks = bb:ReadDouble()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteDouble( o.ticks )
    end
}
BBuffer.Register( RPC_Generic_Pong )