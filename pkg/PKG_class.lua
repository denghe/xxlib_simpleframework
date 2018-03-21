
PKG_PkgGenMd5_Value = 'aa4225edbd8eab2f73f1a1956c172de2'

--[[
一个请求( 当前限定为 service 与 db 间 ), 通常携带一个流水号. 这是基类
]]
PKG_Request = {
    typeName = "PKG_Request",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = PKG_Request
        o.__index = o
        o.__newindex = o

        o.serial = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.serial = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.serial )
    end
}
BBuffer.Register( PKG_Request )
--[[
一个回应( 当前限定为 service 与 db 间 ), 通常携带一个请求发过来的流水号. 这是基类
]]
PKG_Response = {
    typeName = "PKG_Response",
    typeId = 4,
    Create = function()
        local o = {}
        o.__proto = PKG_Response
        o.__index = o
        o.__newindex = o

        o.requestSerial = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.requestSerial = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.requestSerial )
    end
}
BBuffer.Register( PKG_Response )
PKG_Property = {
    typeName = "PKG_Property",
    typeId = 7,
    Create = function()
        local o = {}
        o.__proto = PKG_Property
        o.__index = o
        o.__newindex = o

        o.name = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.name = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.name )
    end
}
BBuffer.Register( PKG_Property )
List_PKG_Property_ = {
    typeName = "List_PKG_Property_",
    typeId = 12,
    Create = function()
        local o = {}
        o.__proto = List_PKG_Property_
        o.__index = o
        o.__newindex = o
        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
        local f = BBuffer.ReadObject
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteObject
        for i = 1, len do
			f( bb, o[ i ] )
		end
    end
}
BBuffer.Register( List_PKG_Property_ )
PKG_UserInfo = {
    typeName = "PKG_UserInfo",
    typeId = 13,
    Create = function()
        local o = {}
        o.__proto = PKG_UserInfo
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int64
        o.props = null -- List_PKG_Property_
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt64()
        o.props = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt64( o.id )
        bb:WriteObject( o.props )
    end
}
BBuffer.Register( PKG_UserInfo )
--[[
请求进入服务器
]]
PKG_Client_Server_Join = {
    typeName = "PKG_Client_Server_Join",
    typeId = 14,
    Create = function()
        local o = {}
        o.__proto = PKG_Client_Server_Join
        o.__index = o
        o.__newindex = o

        o.username = null -- String
        o.password = null -- String
        setmetatable( o, PKG_Request.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        local ReadObject = bb.ReadObject
        o.username = ReadObject( bb )
        o.password = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.username )
        WriteObject( bb, o.password )
    end
}
BBuffer.Register( PKG_Client_Server_Join )
--[[
发消息
]]
PKG_Client_Server_Message = {
    typeName = "PKG_Client_Server_Message",
    typeId = 15,
    Create = function()
        local o = {}
        o.__proto = PKG_Client_Server_Message
        o.__index = o
        o.__newindex = o

        o.text = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.text = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.text )
    end
}
BBuffer.Register( PKG_Client_Server_Message )
--[[
主动退出
]]
PKG_Client_Server_Logout = {
    typeName = "PKG_Client_Server_Logout",
    typeId = 16,
    Create = function()
        local o = {}
        o.__proto = PKG_Client_Server_Logout
        o.__index = o
        o.__newindex = o

        o.ui = null -- PKG_UserInfo
        return o
    end,
    FromBBuffer = function( bb, o )
        o.ui = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.ui )
    end
}
BBuffer.Register( PKG_Client_Server_Logout )
--[[
进入成功, 返回用户信息
]]
PKG_Server_Client_JoinSuccess = {
    typeName = "PKG_Server_Client_JoinSuccess",
    typeId = 17,
    Create = function()
        local o = {}
        o.__proto = PKG_Server_Client_JoinSuccess
        o.__index = o
        o.__newindex = o

        o.self = null -- PKG_UserInfo
        o.users = null -- List_PKG_UserInfo_
        setmetatable( o, PKG_Response.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        local ReadObject = bb.ReadObject
        o.self = ReadObject( bb )
        o.users = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.self )
        WriteObject( bb, o.users )
    end
}
BBuffer.Register( PKG_Server_Client_JoinSuccess )
List_PKG_UserInfo_ = {
    typeName = "List_PKG_UserInfo_",
    typeId = 18,
    Create = function()
        local o = {}
        o.__proto = List_PKG_UserInfo_
        o.__index = o
        o.__newindex = o
        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
        local f = BBuffer.ReadObject
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteObject
        for i = 1, len do
			f( bb, o[ i ] )
		end
    end
}
BBuffer.Register( List_PKG_UserInfo_ )
--[[
进入失败, 返回错误信息
]]
PKG_Server_Client_JoinFail = {
    typeName = "PKG_Server_Client_JoinFail",
    typeId = 19,
    Create = function()
        local o = {}
        o.__proto = PKG_Server_Client_JoinFail
        o.__index = o
        o.__newindex = o

        o.reason = null -- String
        setmetatable( o, PKG_Response.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.reason = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteObject( o.reason )
    end
}
BBuffer.Register( PKG_Server_Client_JoinFail )
--[[
推送文字消息
]]
PKG_Server_Client_PushJoin = {
    typeName = "PKG_Server_Client_PushJoin",
    typeId = 20,
    Create = function()
        local o = {}
        o.__proto = PKG_Server_Client_PushJoin
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt64( o.id )
    end
}
BBuffer.Register( PKG_Server_Client_PushJoin )
--[[
推送文字消息
]]
PKG_Server_Client_PushMessage = {
    typeName = "PKG_Server_Client_PushMessage",
    typeId = 21,
    Create = function()
        local o = {}
        o.__proto = PKG_Server_Client_PushMessage
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int64
        o.text = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt64()
        o.text = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt64( o.id )
        bb:WriteObject( o.text )
    end
}
BBuffer.Register( PKG_Server_Client_PushMessage )
--[[
推送退出消息
]]
PKG_Server_Client_PushLogout = {
    typeName = "PKG_Server_Client_PushLogout",
    typeId = 22,
    Create = function()
        local o = {}
        o.__proto = PKG_Server_Client_PushLogout
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int64
        --[[
        退出原因( 主动? 掉线?
        ]]
        o.reason = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt64()
        o.reason = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt64( o.id )
        bb:WriteObject( o.reason )
    end
}
BBuffer.Register( PKG_Server_Client_PushLogout )