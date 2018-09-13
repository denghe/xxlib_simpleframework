
LOGDB_PkgGenMd5_Value = 'fc66591b1d743ab8a0c7b6e5e6196f14'

--[[
日志级别
]]
LOGDB_Level = {
    Off = 0,
    Fatal = 1,
    Error = 2,
    Warning = 3,
    Info = 4,
    Debug = 5,
    All = 6
}
--[[
对应 log 日志表
]]
LOGDB_Log = {
    typeName = "LOGDB_Log",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = LOGDB_Log
        o.__index = o
        o.__newindex = o

        --[[
        自增主键
        ]]
        o.id = 0 -- Int64
        --[[
        日志级别
        ]]
        o.level = 0 -- LOGDB_Level
        --[[
        发生时间
        ]]
        o.time = 0 -- Int64
        --[[
        机器名
        ]]
        o.machine = null -- String
        --[[
        进程/服务名
        ]]
        o.service = null -- String
        --[[
        进程/实例id
        ]]
        o.instanceId = null -- String
        --[[
        标题
        ]]
        o.title = null -- String
        --[[
        操作代码
        ]]
        o.opcode = 0 -- Int64
        --[[
        日志明细
        ]]
        o.desc = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt64 = bb.ReadInt64
        local ReadObject = bb.ReadObject
        o.id = ReadInt64( bb )
        o.level = bb:ReadInt32()
        o.time = ReadInt64( bb )
        o.machine = ReadObject( bb )
        o.service = ReadObject( bb )
        o.instanceId = ReadObject( bb )
        o.title = ReadObject( bb )
        o.opcode = ReadInt64( bb )
        o.desc = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt64 = bb.WriteInt64
        local WriteObject = bb.WriteObject
        WriteInt64( bb, o.id )
        bb:WriteInt32( o.level )
        WriteInt64( bb, o.time )
        WriteObject( bb, o.machine )
        WriteObject( bb, o.service )
        WriteObject( bb, o.instanceId )
        WriteObject( bb, o.title )
        WriteInt64( bb, o.opcode )
        WriteObject( bb, o.desc )
    end
}
BBuffer.Register( LOGDB_Log )