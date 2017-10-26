
--[[
账号获取时所用的排序规则
]]
PKG_Manage_AccountSorts = {
    id_asc = 0,
    id_desc = 1,
    username_asc = 2,
    username_desc = 3
}
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
--[[
成功
]]
PKG_Success = {
    typeName = "PKG_Success",
    typeId = 5,
    Create = function()
        local o = {}
        o.__proto = PKG_Success
        o.__index = o
        o.__newindex = o

        setmetatable( o, PKG_Response.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
    end
}
BBuffer.Register( PKG_Success )
--[[
失败
]]
PKG_Fail = {
    typeName = "PKG_Fail",
    typeId = 6,
    Create = function()
        local o = {}
        o.__proto = PKG_Fail
        o.__index = o
        o.__newindex = o

        local null = _G.null
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
BBuffer.Register( PKG_Fail )
PKG_Property = {
    typeName = "PKG_Property",
    typeId = 7,
    Create = function()
        local o = {}
        o.__proto = PKG_Property
        o.__index = o
        o.__newindex = o

        local null = _G.null
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
PKG_Property_long = {
    typeName = "PKG_Property_long",
    typeId = 8,
    Create = function()
        local o = {}
        o.__proto = PKG_Property_long
        o.__index = o
        o.__newindex = o

        o.value = 0 -- Int64
        setmetatable( o, PKG_Property.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.value = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteInt64( o.value )
    end
}
BBuffer.Register( PKG_Property_long )
PKG_Property_double = {
    typeName = "PKG_Property_double",
    typeId = 9,
    Create = function()
        local o = {}
        o.__proto = PKG_Property_double
        o.__index = o
        o.__newindex = o

        o.value = 0 -- Double
        setmetatable( o, PKG_Property.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.value = bb:ReadDouble()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteDouble( o.value )
    end
}
BBuffer.Register( PKG_Property_double )
PKG_Property_string = {
    typeName = "PKG_Property_string",
    typeId = 10,
    Create = function()
        local o = {}
        o.__proto = PKG_Property_string
        o.__index = o
        o.__newindex = o

        local null = _G.null
        o.value = null -- String
        setmetatable( o, PKG_Property.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.value = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteObject( o.value )
    end
}
BBuffer.Register( PKG_Property_string )
PKG_Properties = {
    typeName = "PKG_Properties",
    typeId = 11,
    Create = function()
        local o = {}
        o.__proto = PKG_Properties
        o.__index = o
        o.__newindex = o

        local null = _G.null
        o.value = null -- List_PKG_Property_
        setmetatable( o, PKG_Property.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.value = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteObject( o.value )
    end
}
BBuffer.Register( PKG_Properties )
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

        local null = _G.null
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

        local null = _G.null
        o.username = null -- String
        o.password = null -- String
        setmetatable( o, PKG_Request.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        local ReadObject = bb:ReadObject
        o.username = ReadObject( bb )
        o.password = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        local WriteObject = bb:WriteObject
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

        local null = _G.null
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

        return o
    end,
    FromBBuffer = function( bb, o )
    end,
    ToBBuffer = function( bb, o )
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

        local null = _G.null
        o.self = null -- PKG_UserInfo
        o.users = null -- List_PKG_UserInfo_
        setmetatable( o, PKG_Response.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        local ReadObject = bb:ReadObject
        o.self = ReadObject( bb )
        o.users = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        local WriteObject = bb:WriteObject
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

        local null = _G.null
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

        local null = _G.null
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

        local null = _G.null
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
--[[
请求进入服务器
]]
PKG_Manage_DB_Login = {
    typeName = "PKG_Manage_DB_Login",
    typeId = 23,
    Create = function()
        local o = {}
        o.__proto = PKG_Manage_DB_Login
        o.__index = o
        o.__newindex = o

        local null = _G.null
        o.username = null -- String
        o.password = null -- String
        setmetatable( o, PKG_Request.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        local ReadObject = bb:ReadObject
        o.username = ReadObject( bb )
        o.password = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        local WriteObject = bb:WriteObject
        WriteObject( bb, o.username )
        WriteObject( bb, o.password )
    end
}
BBuffer.Register( PKG_Manage_DB_Login )
--[[
注销式退出
]]
PKG_Manage_DB_Logout = {
    typeName = "PKG_Manage_DB_Logout",
    typeId = 24,
    Create = function()
        local o = {}
        o.__proto = PKG_Manage_DB_Logout
        o.__index = o
        o.__newindex = o

        setmetatable( o, PKG_Request.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
    end
}
BBuffer.Register( PKG_Manage_DB_Logout )
--[[
拉管理员账号列表的 id 列表( 需要相关权限 )
]]
PKG_Manage_DB_SelectManageAccountIds = {
    typeName = "PKG_Manage_DB_SelectManageAccountIds",
    typeId = 25,
    Create = function()
        local o = {}
        o.__proto = PKG_Manage_DB_SelectManageAccountIds
        o.__index = o
        o.__newindex = o

        --[[
        限制获取的 id 总个数
        ]]
        o.limit = 0 -- Int32
        --[[
        字段排序规则( 当前先只支持对一个字段排序 )
        ]]
        o.sort = 0 -- PKG_Manage_AccountSorts
        setmetatable( o, PKG_Request.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.limit = bb:ReadInt32()
        o.sort = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteInt32( o.limit )
        bb:WriteObject( o.sort )
    end
}
BBuffer.Register( PKG_Manage_DB_SelectManageAccountIds )
--[[
拉管理员账号列表( 需要相关权限 )
]]
PKG_Manage_DB_SelectManageAccounts = {
    typeName = "PKG_Manage_DB_SelectManageAccounts",
    typeId = 26,
    Create = function()
        local o = {}
        o.__proto = PKG_Manage_DB_SelectManageAccounts
        o.__index = o
        o.__newindex = o

        local null = _G.null
        --[[
        要拉的 id 列表
        ]]
        o.ids = null -- List_Int64_
        setmetatable( o, PKG_Request.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.ids = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteObject( o.ids )
    end
}
BBuffer.Register( PKG_Manage_DB_SelectManageAccounts )
List_Int64_ = {
    typeName = "List_Int64_",
    typeId = 27,
    Create = function()
        local o = {}
        o.__proto = List_Int64_
        o.__index = o
        o.__newindex = o
        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
        local f = BBuffer.ReadInt64
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteInt64
        for i = 1, len do
			f( bb, o[ i ] )
		end
    end
}
BBuffer.Register( List_Int64_ )
--[[
账号
]]
PKG_Manage_Account = {
    typeName = "PKG_Manage_Account",
    typeId = 28,
    Create = function()
        local o = {}
        o.__proto = PKG_Manage_Account
        o.__index = o
        o.__newindex = o

        local null = _G.null
        o.id = 0 -- Int64
        o.username = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt64()
        o.username = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt64( o.id )
        bb:WriteObject( o.username )
    end
}
BBuffer.Register( PKG_Manage_Account )
--[[
身份
]]
PKG_Manage_Role = {
    typeName = "PKG_Manage_Role",
    typeId = 29,
    Create = function()
        local o = {}
        o.__proto = PKG_Manage_Role
        o.__index = o
        o.__newindex = o

        local null = _G.null
        o.id = 0 -- Int32
        o.name = null -- String
        o.desc = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb:ReadObject
        o.id = bb:ReadInt32()
        o.name = ReadObject( bb )
        o.desc = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb:WriteObject
        bb:WriteInt32( o.id )
        WriteObject( bb, o.name )
        WriteObject( bb, o.desc )
    end
}
BBuffer.Register( PKG_Manage_Role )
--[[
权限
]]
PKG_Manage_Permission = {
    typeName = "PKG_Manage_Permission",
    typeId = 30,
    Create = function()
        local o = {}
        o.__proto = PKG_Manage_Permission
        o.__index = o
        o.__newindex = o

        local null = _G.null
        o.id = 0 -- Int32
        o.group = null -- String
        o.name = null -- String
        o.desc = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb:ReadObject
        o.id = bb:ReadInt32()
        o.group = ReadObject( bb )
        o.name = ReadObject( bb )
        o.desc = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb:WriteObject
        bb:WriteInt32( o.id )
        WriteObject( bb, o.group )
        WriteObject( bb, o.name )
        WriteObject( bb, o.desc )
    end
}
BBuffer.Register( PKG_Manage_Permission )
--[[
管理人员 & 身份 绑定
]]
PKG_Manage_BindAccountRole = {
    typeName = "PKG_Manage_BindAccountRole",
    typeId = 31,
    Create = function()
        local o = {}
        o.__proto = PKG_Manage_BindAccountRole
        o.__index = o
        o.__newindex = o

        o.account_id = 0 -- Int32
        o.role_id = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb:ReadInt32
        o.account_id = ReadInt32( bb )
        o.role_id = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb:WriteInt32
        WriteInt32( bb, o.account_id )
        WriteInt32( bb, o.role_id )
    end
}
BBuffer.Register( PKG_Manage_BindAccountRole )
--[[
身份 & 权限 绑定
]]
PKG_Manage_BindRolePermission = {
    typeName = "PKG_Manage_BindRolePermission",
    typeId = 32,
    Create = function()
        local o = {}
        o.__proto = PKG_Manage_BindRolePermission
        o.__index = o
        o.__newindex = o

        o.role_id = 0 -- Int32
        o.permission_id = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb:ReadInt32
        o.role_id = ReadInt32( bb )
        o.permission_id = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb:WriteInt32
        WriteInt32( bb, o.role_id )
        WriteInt32( bb, o.permission_id )
    end
}
BBuffer.Register( PKG_Manage_BindRolePermission )
--[[
登录成功
]]
PKG_DB_Manage_LoginSuccess = {
    typeName = "PKG_DB_Manage_LoginSuccess",
    typeId = 33,
    Create = function()
        local o = {}
        o.__proto = PKG_DB_Manage_LoginSuccess
        o.__index = o
        o.__newindex = o

        local null = _G.null
        o.id = 0 -- Int64
        o.token = null -- String
        setmetatable( o, PKG_Success.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.id = bb:ReadInt64()
        o.token = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteInt64( o.id )
        bb:WriteObject( o.token )
    end
}
BBuffer.Register( PKG_DB_Manage_LoginSuccess )
--[[
登录失败
]]
PKG_DB_Manage_LoginFail = {
    typeName = "PKG_DB_Manage_LoginFail",
    typeId = 34,
    Create = function()
        local o = {}
        o.__proto = PKG_DB_Manage_LoginFail
        o.__index = o
        o.__newindex = o

        setmetatable( o, PKG_Fail.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
    end
}
BBuffer.Register( PKG_DB_Manage_LoginFail )
--[[
注销成功
]]
PKG_DB_Manage_LogoutSuccess = {
    typeName = "PKG_DB_Manage_LogoutSuccess",
    typeId = 35,
    Create = function()
        local o = {}
        o.__proto = PKG_DB_Manage_LogoutSuccess
        o.__index = o
        o.__newindex = o

        setmetatable( o, PKG_Success.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
    end
}
BBuffer.Register( PKG_DB_Manage_LogoutSuccess )
--[[
注销失败
]]
PKG_DB_Manage_LogoutFail = {
    typeName = "PKG_DB_Manage_LogoutFail",
    typeId = 36,
    Create = function()
        local o = {}
        o.__proto = PKG_DB_Manage_LogoutFail
        o.__index = o
        o.__newindex = o

        setmetatable( o, PKG_Fail.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
    end
}
BBuffer.Register( PKG_DB_Manage_LogoutFail )
--[[
拉管理员账号列表的 id 列表 成功
]]
PKG_DB_Manage_SelectManageAccountIdsSuccess = {
    typeName = "PKG_DB_Manage_SelectManageAccountIdsSuccess",
    typeId = 37,
    Create = function()
        local o = {}
        o.__proto = PKG_DB_Manage_SelectManageAccountIdsSuccess
        o.__index = o
        o.__newindex = o

        local null = _G.null
        o.ids = null -- List_Int64_
        setmetatable( o, PKG_Success.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.ids = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteObject( o.ids )
    end
}
BBuffer.Register( PKG_DB_Manage_SelectManageAccountIdsSuccess )
--[[
拉管理员账号列表的 id 列表 失败
]]
PKG_DB_Manage_SelectManageAccountIdsFail = {
    typeName = "PKG_DB_Manage_SelectManageAccountIdsFail",
    typeId = 38,
    Create = function()
        local o = {}
        o.__proto = PKG_DB_Manage_SelectManageAccountIdsFail
        o.__index = o
        o.__newindex = o

        setmetatable( o, PKG_Fail.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
    end
}
BBuffer.Register( PKG_DB_Manage_SelectManageAccountIdsFail )
--[[
拉管理员账号列表 成功
]]
PKG_DB_Manage_SelectManageAccountsSuccess = {
    typeName = "PKG_DB_Manage_SelectManageAccountsSuccess",
    typeId = 39,
    Create = function()
        local o = {}
        o.__proto = PKG_DB_Manage_SelectManageAccountsSuccess
        o.__index = o
        o.__newindex = o

        local null = _G.null
        o.rows = null -- List_PKG_Manage_Account_
        setmetatable( o, PKG_Success.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.rows = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteObject( o.rows )
    end
}
BBuffer.Register( PKG_DB_Manage_SelectManageAccountsSuccess )
List_PKG_Manage_Account_ = {
    typeName = "List_PKG_Manage_Account_",
    typeId = 40,
    Create = function()
        local o = {}
        o.__proto = List_PKG_Manage_Account_
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
BBuffer.Register( List_PKG_Manage_Account_ )
--[[
拉管理员账号列表 失败
]]
PKG_DB_Manage_SelectManageAccountsFail = {
    typeName = "PKG_DB_Manage_SelectManageAccountsFail",
    typeId = 41,
    Create = function()
        local o = {}
        o.__proto = PKG_DB_Manage_SelectManageAccountsFail
        o.__index = o
        o.__newindex = o

        setmetatable( o, PKG_Fail.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
    end
}
BBuffer.Register( PKG_DB_Manage_SelectManageAccountsFail )