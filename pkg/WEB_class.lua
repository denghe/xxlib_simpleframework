
WEB_PkgGenMd5_Value = 'cd86e1871b59a216ef09ac65f285ddc1'

--[[
管理人员
]]
WEB_Manager = {
    typeName = "WEB_Manager",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = WEB_Manager
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        --[[
        当前令牌( 页面每次 Load 时与存放于 Session 中的值做对比用 )
        ]]
        o.token = null -- String
        --[[
        最后访问的时间点( 页面每次 Load 时更新该值, 用于超时判断 )
        ]]
        o.lastVisitTime = null -- DateTime
        setmetatable( o, WEB_Tables_manager.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        local ReadObject = bb.ReadObject
        o.token = ReadObject( bb )
        o.lastVisitTime = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.token )
        WriteObject( bb, o.lastVisitTime )
    end
}
BBuffer.Register( WEB_Manager )
--[[
管理人员表
]]
WEB_Tables_manager = {
    typeName = "WEB_Tables_manager",
    typeId = 4,
    Create = function()
        local o = {}
        o.__proto = WEB_Tables_manager
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.id = 0 -- Int32
        o.username = null -- String
        o.password = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = bb:ReadInt32()
        o.username = ReadObject( bb )
        o.password = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        bb:WriteInt32( o.id )
        WriteObject( bb, o.username )
        WriteObject( bb, o.password )
    end
}
BBuffer.Register( WEB_Tables_manager )
--[[
身份
]]
WEB_Role = {
    typeName = "WEB_Role",
    typeId = 5,
    Create = function()
        local o = {}
        o.__proto = WEB_Role
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        setmetatable( o, WEB_Tables_role.Create() )
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
BBuffer.Register( WEB_Role )
--[[
管理人员身份表
]]
WEB_Tables_role = {
    typeName = "WEB_Tables_role",
    typeId = 11,
    Create = function()
        local o = {}
        o.__proto = WEB_Tables_role
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.id = 0 -- Int32
        o.name = null -- String
        o.desc = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = bb:ReadInt32()
        o.name = ReadObject( bb )
        o.desc = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        bb:WriteInt32( o.id )
        WriteObject( bb, o.name )
        WriteObject( bb, o.desc )
    end
}
BBuffer.Register( WEB_Tables_role )
--[[
权限
]]
WEB_Permission = {
    typeName = "WEB_Permission",
    typeId = 6,
    Create = function()
        local o = {}
        o.__proto = WEB_Permission
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        setmetatable( o, WEB_Tables_permission.Create() )
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
BBuffer.Register( WEB_Permission )
--[[
权限表
]]
WEB_Tables_permission = {
    typeName = "WEB_Tables_permission",
    typeId = 10,
    Create = function()
        local o = {}
        o.__proto = WEB_Tables_permission
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.id = 0 -- Int32
        o.group = null -- String
        o.name = null -- String
        o.desc = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = bb:ReadInt32()
        o.group = ReadObject( bb )
        o.name = ReadObject( bb )
        o.desc = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        bb:WriteInt32( o.id )
        WriteObject( bb, o.group )
        WriteObject( bb, o.name )
        WriteObject( bb, o.desc )
    end
}
BBuffer.Register( WEB_Tables_permission )
--[[
管理人员_身份_绑定
]]
WEB_BindManagerRole = {
    typeName = "WEB_BindManagerRole",
    typeId = 7,
    Create = function()
        local o = {}
        o.__proto = WEB_BindManagerRole
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        setmetatable( o, WEB_Tables_manager_role.Create() )
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
BBuffer.Register( WEB_BindManagerRole )
--[[
管理人员_身份_绑定表
]]
WEB_Tables_manager_role = {
    typeName = "WEB_Tables_manager_role",
    typeId = 9,
    Create = function()
        local o = {}
        o.__proto = WEB_Tables_manager_role
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.manager_id = 0 -- Int32
        o.role_id = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.manager_id = ReadInt32( bb )
        o.role_id = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.manager_id )
        WriteInt32( bb, o.role_id )
    end
}
BBuffer.Register( WEB_Tables_manager_role )
--[[
身份_权限_绑定
]]
WEB_BindRolePermission = {
    typeName = "WEB_BindRolePermission",
    typeId = 8,
    Create = function()
        local o = {}
        o.__proto = WEB_BindRolePermission
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        setmetatable( o, WEB_Tables_role_permission.Create() )
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
BBuffer.Register( WEB_BindRolePermission )
--[[
身份_权限_绑定表
]]
WEB_Tables_role_permission = {
    typeName = "WEB_Tables_role_permission",
    typeId = 12,
    Create = function()
        local o = {}
        o.__proto = WEB_Tables_role_permission
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.role_id = 0 -- Int32
        o.permission_id = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.role_id = ReadInt32( bb )
        o.permission_id = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.role_id )
        WriteInt32( bb, o.permission_id )
    end
}
BBuffer.Register( WEB_Tables_role_permission )
WEB_Generic_SomeLists = {
    typeName = "WEB_Generic_SomeLists",
    typeId = 13,
    Create = function()
        local o = {}
        o.__proto = WEB_Generic_SomeLists
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.ints = null -- List_Int32_
        o.longs = null -- List_Int64_
        o.strings = null -- List_String_
        o.Managers = null -- List_WEB_Manager_
        o.Roles = null -- List_WEB_Role_
        o.Permissions = null -- List_WEB_Permission_
        o.BindManagerRoles = null -- List_WEB_BindManagerRole_
        o.BindRolePermissions = null -- List_WEB_BindRolePermission_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.ints = ReadObject( bb )
        o.longs = ReadObject( bb )
        o.strings = ReadObject( bb )
        o.Managers = ReadObject( bb )
        o.Roles = ReadObject( bb )
        o.Permissions = ReadObject( bb )
        o.BindManagerRoles = ReadObject( bb )
        o.BindRolePermissions = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.ints )
        WriteObject( bb, o.longs )
        WriteObject( bb, o.strings )
        WriteObject( bb, o.Managers )
        WriteObject( bb, o.Roles )
        WriteObject( bb, o.Permissions )
        WriteObject( bb, o.BindManagerRoles )
        WriteObject( bb, o.BindRolePermissions )
    end
}
BBuffer.Register( WEB_Generic_SomeLists )
List_Int32_ = {
    typeName = "List_Int32_",
    typeId = 14,
    Create = function()
        local o = {}
        o.__proto = List_Int32_
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
        local f = BBuffer.ReadInt32
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteInt32
        for i = 1, len do
			f( bb, o[ i ] )
		end
    end
}
BBuffer.Register( List_Int32_ )
List_Int64_ = {
    typeName = "List_Int64_",
    typeId = 15,
    Create = function()
        local o = {}
        o.__proto = List_Int64_
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end

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
List_String_ = {
    typeName = "List_String_",
    typeId = 16,
    Create = function()
        local o = {}
        o.__proto = List_String_
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end

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
BBuffer.Register( List_String_ )
List_WEB_Manager_ = {
    typeName = "List_WEB_Manager_",
    typeId = 17,
    Create = function()
        local o = {}
        o.__proto = List_WEB_Manager_
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end

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
BBuffer.Register( List_WEB_Manager_ )
List_WEB_Role_ = {
    typeName = "List_WEB_Role_",
    typeId = 18,
    Create = function()
        local o = {}
        o.__proto = List_WEB_Role_
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end

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
BBuffer.Register( List_WEB_Role_ )
List_WEB_Permission_ = {
    typeName = "List_WEB_Permission_",
    typeId = 19,
    Create = function()
        local o = {}
        o.__proto = List_WEB_Permission_
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end

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
BBuffer.Register( List_WEB_Permission_ )
List_WEB_BindManagerRole_ = {
    typeName = "List_WEB_BindManagerRole_",
    typeId = 20,
    Create = function()
        local o = {}
        o.__proto = List_WEB_BindManagerRole_
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end

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
BBuffer.Register( List_WEB_BindManagerRole_ )
List_WEB_BindRolePermission_ = {
    typeName = "List_WEB_BindRolePermission_",
    typeId = 21,
    Create = function()
        local o = {}
        o.__proto = List_WEB_BindRolePermission_
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end

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
BBuffer.Register( List_WEB_BindRolePermission_ )