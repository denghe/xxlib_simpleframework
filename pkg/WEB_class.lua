
WEB_PkgGenMd5_Value = '493c5cdfff1560d487bb7c021cd017bc'

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