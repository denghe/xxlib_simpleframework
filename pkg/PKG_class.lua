
PKG_PkgGenMd5_Value = 'dde27285e5784277142135589ddf9e51'

PKG_Foo = {
    typeName = "PKG_Foo",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = PKG_Foo
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        o.age = null -- NullableInt32
        o.info = null -- String
        o.childs = null -- List_PKG_Foo_
        o.o = null -- Object
        o.os = null -- List_Object_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = bb:ReadInt32()
        o.age = ReadObject( bb )
        o.info = ReadObject( bb )
        o.childs = ReadObject( bb )
        o.o = ReadObject( bb )
        o.os = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        bb:WriteInt32( o.id )
        WriteObject( bb, o.age )
        WriteObject( bb, o.info )
        WriteObject( bb, o.childs )
        WriteObject( bb, o.o )
        WriteObject( bb, o.os )
    end
}
BBuffer.Register( PKG_Foo )
List_PKG_Foo_ = {
    typeName = "List_PKG_Foo_",
    typeId = 4,
    Create = function()
        local o = {}
        o.__proto = List_PKG_Foo_
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
BBuffer.Register( List_PKG_Foo_ )
List_Object_ = {
    typeName = "List_Object_",
    typeId = 5,
    Create = function()
        local o = {}
        o.__proto = List_Object_
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
BBuffer.Register( List_Object_ )
PKG_FooEx = {
    typeName = "PKG_FooEx",
    typeId = 6,
    Create = function()
        local o = {}
        o.__proto = PKG_FooEx
        o.__index = o
        o.__newindex = o

        setmetatable( o, PKG_Foo.Create() )
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
BBuffer.Register( PKG_FooEx )
PKG_Node = {
    typeName = "PKG_Node",
    typeId = 7,
    Create = function()
        local o = {}
        o.__proto = PKG_Node
        o.__index = o
        o.__newindex = o

        o.parent = null -- PKG_Node
        o.childs = null -- List_PKG_Node_
        setmetatable( o, PKG_Tables_node.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        local ReadObject = bb.ReadObject
        o.parent = ReadObject( bb )
        o.childs = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.parent )
        WriteObject( bb, o.childs )
    end
}
BBuffer.Register( PKG_Node )
PKG_Tables_node = {
    typeName = "PKG_Tables_node",
    typeId = 8,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_node
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        o.pid = null -- NullableInt32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt32()
        o.pid = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.id )
        bb:WriteObject( o.pid )
    end
}
BBuffer.Register( PKG_Tables_node )
List_PKG_Node_ = {
    typeName = "List_PKG_Node_",
    typeId = 9,
    Create = function()
        local o = {}
        o.__proto = List_PKG_Node_
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
BBuffer.Register( List_PKG_Node_ )
PKG_DataSet = {
    typeName = "PKG_DataSet",
    typeId = 10,
    Create = function()
        local o = {}
        o.__proto = PKG_DataSet
        o.__index = o
        o.__newindex = o

        o.tables = null -- List_PKG_Table_
        return o
    end,
    FromBBuffer = function( bb, o )
        o.tables = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.tables )
    end
}
BBuffer.Register( PKG_DataSet )
List_PKG_Table_ = {
    typeName = "List_PKG_Table_",
    typeId = 11,
    Create = function()
        local o = {}
        o.__proto = List_PKG_Table_
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
BBuffer.Register( List_PKG_Table_ )
PKG_Table = {
    typeName = "PKG_Table",
    typeId = 12,
    Create = function()
        local o = {}
        o.__proto = PKG_Table
        o.__index = o
        o.__newindex = o

        o.parent = null -- PKG_DataSet
        o.name = null -- String
        o.columns = null -- List_PKG_TableColumn_
        o.rows = null -- List_PKG_TableRow_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.parent = ReadObject( bb )
        o.name = ReadObject( bb )
        o.columns = ReadObject( bb )
        o.rows = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.parent )
        WriteObject( bb, o.name )
        WriteObject( bb, o.columns )
        WriteObject( bb, o.rows )
    end
}
BBuffer.Register( PKG_Table )
List_PKG_TableColumn_ = {
    typeName = "List_PKG_TableColumn_",
    typeId = 13,
    Create = function()
        local o = {}
        o.__proto = List_PKG_TableColumn_
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
BBuffer.Register( List_PKG_TableColumn_ )
PKG_TableColumn = {
    typeName = "PKG_TableColumn",
    typeId = 14,
    Create = function()
        local o = {}
        o.__proto = PKG_TableColumn
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
BBuffer.Register( PKG_TableColumn )
List_PKG_TableRow_ = {
    typeName = "List_PKG_TableRow_",
    typeId = 15,
    Create = function()
        local o = {}
        o.__proto = List_PKG_TableRow_
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
BBuffer.Register( List_PKG_TableRow_ )
PKG_TableRow = {
    typeName = "PKG_TableRow",
    typeId = 16,
    Create = function()
        local o = {}
        o.__proto = PKG_TableRow
        o.__index = o
        o.__newindex = o

        o.values = null -- List_PKG_TableRowValue_
        return o
    end,
    FromBBuffer = function( bb, o )
        o.values = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.values )
    end
}
BBuffer.Register( PKG_TableRow )
List_PKG_TableRowValue_ = {
    typeName = "List_PKG_TableRowValue_",
    typeId = 17,
    Create = function()
        local o = {}
        o.__proto = List_PKG_TableRowValue_
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
BBuffer.Register( List_PKG_TableRowValue_ )
PKG_TableRowValue = {
    typeName = "PKG_TableRowValue",
    typeId = 18,
    Create = function()
        local o = {}
        o.__proto = PKG_TableRowValue
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
    end,
    ToBBuffer = function( bb, o )
    end
}
BBuffer.Register( PKG_TableRowValue )
PKG_TableRowValue_Int = {
    typeName = "PKG_TableRowValue_Int",
    typeId = 19,
    Create = function()
        local o = {}
        o.__proto = PKG_TableRowValue_Int
        o.__index = o
        o.__newindex = o

        o.value = 0 -- Int32
        setmetatable( o, PKG_TableRowValue.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.value = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteInt32( o.value )
    end
}
BBuffer.Register( PKG_TableRowValue_Int )
PKG_TableRowValue_NullableInt = {
    typeName = "PKG_TableRowValue_NullableInt",
    typeId = 20,
    Create = function()
        local o = {}
        o.__proto = PKG_TableRowValue_NullableInt
        o.__index = o
        o.__newindex = o

        o.value = null -- NullableInt32
        setmetatable( o, PKG_TableRowValue.Create() )
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
BBuffer.Register( PKG_TableRowValue_NullableInt )
PKG_TableRowValue_String = {
    typeName = "PKG_TableRowValue_String",
    typeId = 21,
    Create = function()
        local o = {}
        o.__proto = PKG_TableRowValue_String
        o.__index = o
        o.__newindex = o

        o.value = null -- String
        setmetatable( o, PKG_TableRowValue.Create() )
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
BBuffer.Register( PKG_TableRowValue_String )
--[[
玩家账号表( 基表 )
]]
PKG_Tables_account = {
    typeName = "PKG_Tables_account",
    typeId = 22,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_account
        o.__index = o
        o.__newindex = o

        --[[
        用户Id ( 随机 8 位整数 )
        ]]
        o.id = 0 -- Int32
        --[[
        原始用户名 唯一( GUID )
        ]]
        o.username = null -- String
        --[[
        昵称 唯一( 默认用某种规则生成 )
        ]]
        o.nickname = null -- String
        --[[
        头像
        ]]
        o.avatar_id = 0 -- Int32
        --[[
        电话号码 唯一( 默认填充 username GUID )
        ]]
        o.phone = null -- String
        --[[
        密码( 默认为空 )
        ]]
        o.password = null -- String
        --[[
        账户余额( 保留4位小数位, 进部分游戏时会被清0, 结束时会兑换返还 )
        ]]
        o.money = 0 -- Double
        --[[
        保险箱( 玩家可在账户余额间搬运数据 )
        ]]
        o.money_safe = 0 -- Double
        --[[
        累计充值金额
        ]]
        o.total_recharge = 0 -- Double
        --[[
        累计消费金额( 比如在鱼里一共打了多少钱的炮 )
        ]]
        o.total_consume = 0 -- Double
        --[[
        累计提现金额
        ]]
        o.total_withdraw = 0 -- Double
        --[[
        创建时间. epoch 10m 精度. 所有表的这个字段都是这个格式
        ]]
        o.create_time = 0 -- Int64
        --[[
        启用标记( 1 : 启用 )
        ]]
        o.enabled = 0 -- Int32
        --[[
        玩家进入大厅时间(为空 则离开大厅)
        ]]
        o.enter_lobby_time = null -- NullableInt64
        --[[
        正在游戏中的玩家日志id(为空 则离开游戏)
        ]]
        o.log_game_id = null -- NullableInt32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadObject = bb.ReadObject
        local ReadDouble = bb.ReadDouble
        o.id = ReadInt32( bb )
        o.username = ReadObject( bb )
        o.nickname = ReadObject( bb )
        o.avatar_id = ReadInt32( bb )
        o.phone = ReadObject( bb )
        o.password = ReadObject( bb )
        o.money = ReadDouble( bb )
        o.money_safe = ReadDouble( bb )
        o.total_recharge = ReadDouble( bb )
        o.total_consume = ReadDouble( bb )
        o.total_withdraw = ReadDouble( bb )
        o.create_time = bb:ReadInt64()
        o.enabled = ReadInt32( bb )
        o.enter_lobby_time = ReadObject( bb )
        o.log_game_id = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteObject = bb.WriteObject
        local WriteDouble = bb.WriteDouble
        WriteInt32( bb, o.id )
        WriteObject( bb, o.username )
        WriteObject( bb, o.nickname )
        WriteInt32( bb, o.avatar_id )
        WriteObject( bb, o.phone )
        WriteObject( bb, o.password )
        WriteDouble( bb, o.money )
        WriteDouble( bb, o.money_safe )
        WriteDouble( bb, o.total_recharge )
        WriteDouble( bb, o.total_consume )
        WriteDouble( bb, o.total_withdraw )
        bb:WriteInt64( o.create_time )
        WriteInt32( bb, o.enabled )
        WriteObject( bb, o.enter_lobby_time )
        WriteObject( bb, o.log_game_id )
    end
}
BBuffer.Register( PKG_Tables_account )
--[[
在线聊天
]]
PKG_Tables_chat = {
    typeName = "PKG_Tables_chat",
    typeId = 23,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_chat
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        --[[
        内容
        ]]
        o.content = null -- String
        --[[
        聊天人id
        ]]
        o.account_id = 0 -- Int32
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.content = bb:ReadObject()
        o.account_id = ReadInt32( bb )
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        bb:WriteObject( o.content )
        WriteInt32( bb, o.account_id )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_chat )
--[[
通用配置表( 带版本号的键值对集合 )
]]
PKG_Tables_config = {
    typeName = "PKG_Tables_config",
    typeId = 24,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_config
        o.__index = o
        o.__newindex = o

        --[[
        键名
        ]]
        o.key = null -- String
        --[[
        数据类型名
        ]]
        o.datatype_name = null -- String
        --[[
        显示名
        ]]
        o.title = null -- String
        --[[
        说明
        ]]
        o.desc = null -- String
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.key = ReadObject( bb )
        o.datatype_name = ReadObject( bb )
        o.title = ReadObject( bb )
        o.desc = ReadObject( bb )
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.key )
        WriteObject( bb, o.datatype_name )
        WriteObject( bb, o.title )
        WriteObject( bb, o.desc )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_config )
--[[
配置项所对应的值( 保留历史变化, 通常只有最新的那一个生效 )
]]
PKG_Tables_config_value = {
    typeName = "PKG_Tables_config_value",
    typeId = 25,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_config_value
        o.__index = o
        o.__newindex = o

        --[[
        键名
        ]]
        o.key = null -- String
        --[[
        对应 bigint 数据类型的值( int64 )
        ]]
        o.value_bigint = null -- NullableInt64
        --[[
        对应 double 数据类型的值
        ]]
        o.value_double = null -- NullableDouble
        --[[
        对应 varchar 数据类型的值( string )
        ]]
        o.value_varchar = null -- String
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.key = ReadObject( bb )
        o.value_bigint = ReadObject( bb )
        o.value_double = ReadObject( bb )
        o.value_varchar = ReadObject( bb )
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.key )
        WriteObject( bb, o.value_bigint )
        WriteObject( bb, o.value_double )
        WriteObject( bb, o.value_varchar )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_config_value )
--[[
数据类型列表( 主要供 config 用 )
]]
PKG_Tables_datatype = {
    typeName = "PKG_Tables_datatype",
    typeId = 26,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_datatype
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
BBuffer.Register( PKG_Tables_datatype )
--[[
鱼种类表
]]
PKG_Tables_fish = {
    typeName = "PKG_Tables_fish",
    typeId = 27,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_fish
        o.__index = o
        o.__newindex = o

        --[[
        鱼配置id
        ]]
        o.id = 0 -- Int32
        --[[
        名称
        ]]
        o.name = null -- String
        --[[
        打死鱼的金币所得最小基数( 也可理解成倍率 )
        ]]
        o.minCoin = 0 -- Int32
        --[[
         打死鱼的金币所得最大基数( 也可理解成倍率 )
        ]]
        o.maxCoin = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.name = bb:ReadObject()
        o.minCoin = ReadInt32( bb )
        o.maxCoin = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        bb:WriteObject( o.name )
        WriteInt32( bb, o.minCoin )
        WriteInt32( bb, o.maxCoin )
    end
}
BBuffer.Register( PKG_Tables_fish )
--[[
所有游戏列表( 基表 )
]]
PKG_Tables_game = {
    typeName = "PKG_Tables_game",
    typeId = 28,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_game
        o.__index = o
        o.__newindex = o

        --[[
        游戏编号( 非自增, 直接填死 )
        ]]
        o.id = 0 -- Int32
        --[[
        游戏名
        ]]
        o.name = null -- String
        --[[
        类型名( 派生表名 )
        ]]
        o.type_name = null -- String
        --[[
        是否启用 1: true
        ]]
        o.enabled = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadObject = bb.ReadObject
        o.id = ReadInt32( bb )
        o.name = ReadObject( bb )
        o.type_name = ReadObject( bb )
        o.enabled = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteObject = bb.WriteObject
        WriteInt32( bb, o.id )
        WriteObject( bb, o.name )
        WriteObject( bb, o.type_name )
        WriteInt32( bb, o.enabled )
    end
}
BBuffer.Register( PKG_Tables_game )
--[[
game 的 捕鱼游戏 派生表( 存放专属字段 )
]]
PKG_Tables_game_catchfish = {
    typeName = "PKG_Tables_game_catchfish",
    typeId = 29,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_game_catchfish
        o.__index = o
        o.__newindex = o

        --[[
        基表游戏编号
        ]]
        o.id = 0 -- Int32
        --[[
        进出游戏时 money 自动兑换成 coin 要 乘除 的系数
        ]]
        o.exchange_coin_ratio = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.exchange_coin_ratio = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.exchange_coin_ratio )
    end
}
BBuffer.Register( PKG_Tables_game_catchfish )
--[[
玩家于捕鱼游戏的个人配置信息
]]
PKG_Tables_game_catchfish_account = {
    typeName = "PKG_Tables_game_catchfish_account",
    typeId = 30,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_game_catchfish_account
        o.__index = o
        o.__newindex = o

        --[[
        账号id
        ]]
        o.account_id = 0 -- Int32
        --[[
        捕鱼游戏id
        ]]
        o.game_id = 0 -- Int32
        --[[
        级别id
        ]]
        o.level_id = 0 -- Int32
        --[[
        玩家上次游戏退出前用的子弹的倍率( 游戏内金币整数值 )
        ]]
        o.last_bet = 0 -- Int64
        --[[
        玩家上次游戏退出前用的炮台id
        ]]
        o.last_cannon_id = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.account_id = ReadInt32( bb )
        o.game_id = ReadInt32( bb )
        o.level_id = ReadInt32( bb )
        o.last_bet = bb:ReadInt64()
        o.last_cannon_id = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.account_id )
        WriteInt32( bb, o.game_id )
        WriteInt32( bb, o.level_id )
        bb:WriteInt64( o.last_bet )
        WriteInt32( bb, o.last_cannon_id )
    end
}
BBuffer.Register( PKG_Tables_game_catchfish_account )
--[[
捕鱼游戏内炮台skin与累计充值金额的对应表
]]
PKG_Tables_game_catchfish_cannon = {
    typeName = "PKG_Tables_game_catchfish_cannon",
    typeId = 31,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_game_catchfish_cannon
        o.__index = o
        o.__newindex = o

        --[[
        唯一编号 & 炮台Skin
        ]]
        o.id = 0 -- Int32
        --[[
        VIP等级
        ]]
        o.vip_id = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.vip_id = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.vip_id )
    end
}
BBuffer.Register( PKG_Tables_game_catchfish_cannon )
--[[
捕鱼游戏的分级表( 1对多 )
]]
PKG_Tables_game_catchfish_level = {
    typeName = "PKG_Tables_game_catchfish_level",
    typeId = 32,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_game_catchfish_level
        o.__index = o
        o.__newindex = o

        --[[
        游戏id
        ]]
        o.game_id = 0 -- Int32
        --[[
        游戏级别id. 0 1 2....: 新手厅  进阶厅  高手厅 ....
        ]]
        o.level_id = 0 -- Int32
        --[[
        炮值(从)(游戏内金币)
        ]]
        o.min_bet = 0 -- Int64
        --[[
        炮值(到)(游戏内金币)
        ]]
        o.max_bet = 0 -- Int64
        --[[
        最低准入金额
        ]]
        o.min_money = 0 -- Double
        --[[
        是否启用 1: true
        ]]
        o.enabled = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadInt64 = bb.ReadInt64
        o.game_id = ReadInt32( bb )
        o.level_id = ReadInt32( bb )
        o.min_bet = ReadInt64( bb )
        o.max_bet = ReadInt64( bb )
        o.min_money = bb:ReadDouble()
        o.enabled = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteInt64 = bb.WriteInt64
        WriteInt32( bb, o.game_id )
        WriteInt32( bb, o.level_id )
        WriteInt64( bb, o.min_bet )
        WriteInt64( bb, o.max_bet )
        bb:WriteDouble( o.min_money )
        WriteInt32( bb, o.enabled )
    end
}
BBuffer.Register( PKG_Tables_game_catchfish_level )
--[[
捕鱼游戏的分级表 下属房间表( 1对多 )
]]
PKG_Tables_game_catchfish_level_room = {
    typeName = "PKG_Tables_game_catchfish_level_room",
    typeId = 33,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_game_catchfish_level_room
        o.__index = o
        o.__newindex = o

        --[[
        游戏id
        ]]
        o.game_id = 0 -- Int32
        --[[
        游戏级别id
        ]]
        o.level_id = 0 -- Int32
        --[[
        房号( 从 1 开始填充, 新增取最大值+1 )
        ]]
        o.room_id = 0 -- Int32
        --[[
        是否启用 1: true
        ]]
        o.enabled = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.game_id = ReadInt32( bb )
        o.level_id = ReadInt32( bb )
        o.room_id = ReadInt32( bb )
        o.enabled = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.game_id )
        WriteInt32( bb, o.level_id )
        WriteInt32( bb, o.room_id )
        WriteInt32( bb, o.enabled )
    end
}
BBuffer.Register( PKG_Tables_game_catchfish_level_room )
--[[
头像变更历史表
]]
PKG_Tables_log_account_avatar = {
    typeName = "PKG_Tables_log_account_avatar",
    typeId = 34,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_avatar
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        --[[
        帐号id
        ]]
        o.account_id = 0 -- Int32
        --[[
        头像
        ]]
        o.avatar_id = 0 -- Int32
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.account_id = ReadInt32( bb )
        o.avatar_id = ReadInt32( bb )
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.account_id )
        WriteInt32( bb, o.avatar_id )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_avatar )
--[[
帐号启用标记变更历史表
]]
PKG_Tables_log_account_enabled = {
    typeName = "PKG_Tables_log_account_enabled",
    typeId = 35,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_enabled
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        --[[
        帐号id
        ]]
        o.account_id = 0 -- Int32
        --[[
        启用标记
        ]]
        o.enabled = 0 -- Int32
        --[[
        备注( 含修改人 & 原因 )
        ]]
        o.memo = null -- String
        --[[
        日志创建时间( 修改时间 )
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.account_id = ReadInt32( bb )
        o.enabled = ReadInt32( bb )
        o.memo = bb:ReadObject()
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.account_id )
        WriteInt32( bb, o.enabled )
        bb:WriteObject( o.memo )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_enabled )
--[[
保险箱操作日志
]]
PKG_Tables_log_account_money_safe = {
    typeName = "PKG_Tables_log_account_money_safe",
    typeId = 36,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_money_safe
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        --[[
        帐号id
        ]]
        o.account_id = 0 -- Int32
        --[[
        金额( 正数为存入保险箱, 负数为从保险箱提钱 )
        ]]
        o.value = 0 -- Double
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.account_id = ReadInt32( bb )
        o.value = bb:ReadDouble()
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.account_id )
        bb:WriteDouble( o.value )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_money_safe )
--[[
昵称变更历史表
]]
PKG_Tables_log_account_nickname = {
    typeName = "PKG_Tables_log_account_nickname",
    typeId = 37,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_nickname
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        --[[
        帐号id
        ]]
        o.account_id = 0 -- Int32
        --[[
        昵称
        ]]
        o.nickname = null -- String
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.account_id = ReadInt32( bb )
        o.nickname = bb:ReadObject()
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.account_id )
        bb:WriteObject( o.nickname )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_nickname )
--[[
密码变更历史表
]]
PKG_Tables_log_account_password = {
    typeName = "PKG_Tables_log_account_password",
    typeId = 38,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_password
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        --[[
        帐号id
        ]]
        o.account_id = 0 -- Int32
        --[[
        密码
        ]]
        o.password = null -- String
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.account_id = ReadInt32( bb )
        o.password = bb:ReadObject()
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.account_id )
        bb:WriteObject( o.password )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_password )
--[[
电话号码变更历史表
]]
PKG_Tables_log_account_phone = {
    typeName = "PKG_Tables_log_account_phone",
    typeId = 39,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_phone
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        --[[
        帐号id
        ]]
        o.account_id = 0 -- Int32
        --[[
        电话号码
        ]]
        o.phone = null -- String
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.account_id = ReadInt32( bb )
        o.phone = bb:ReadObject()
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.account_id )
        bb:WriteObject( o.phone )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_phone )
--[[
充值记录表
]]
PKG_Tables_log_account_recharge = {
    typeName = "PKG_Tables_log_account_recharge",
    typeId = 40,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_recharge
        o.__index = o
        o.__newindex = o

        --[[
        充值请求原始令牌
        ]]
        o.token = null -- String
        --[[
        帐号id
        ]]
        o.account_id = 0 -- Int32
        --[[
        充值金额
        ]]
        o.money = 0 -- Double
        --[[
        备注( 渠道, 操作人等 )
        ]]
        o.memo = null -- String
        --[[
        日志创建时间( 充值时间 )
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.token = ReadObject( bb )
        o.account_id = bb:ReadInt32()
        o.money = bb:ReadDouble()
        o.memo = ReadObject( bb )
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.token )
        bb:WriteInt32( o.account_id )
        bb:WriteDouble( o.money )
        WriteObject( bb, o.memo )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_recharge )
--[[
玩家实时对局数据日志表(基表 )
]]
PKG_Tables_log_game = {
    typeName = "PKG_Tables_log_game",
    typeId = 41,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        --[[
        游戏id(game表id)
        ]]
        o.game_id = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.game_id = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.game_id )
    end
}
BBuffer.Register( PKG_Tables_log_game )
--[[
捕鱼房内玩家实时对局数据日志表( 数据于玩家进游戏时创建, 出游戏时将继续填充这个表剩余字段 )
]]
PKG_Tables_log_game_catchfish = {
    typeName = "PKG_Tables_log_game_catchfish",
    typeId = 42,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game_catchfish
        o.__index = o
        o.__newindex = o

        --[[
        游戏日志id(log_game表id)
        ]]
        o.id = 0 -- Int32
        --[[
        游戏级别id
        ]]
        o.level_id = 0 -- Int32
        --[[
        房号
        ]]
        o.room_id = 0 -- Int32
        --[[
        账号id
        ]]
        o.account_id = 0 -- Int32
        --[[
        进房时间
        ]]
        o.enter_time = 0 -- Int64
        --[[
        进房时的账户余额
        ]]
        o.enter_money = 0 -- Double
        --[[
        离开时间( 日志记录创建时间 )
        ]]
        o.leave_time = 0 -- Int64
        --[[
        离开时游戏内剩余金币换算成的金额
        ]]
        o.leave_money = 0 -- Double
        --[[
        当次游戏内花掉的金币换算成的金额总量
        ]]
        o.consume = 0 -- Double
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadInt64 = bb.ReadInt64
        local ReadDouble = bb.ReadDouble
        o.id = ReadInt32( bb )
        o.level_id = ReadInt32( bb )
        o.room_id = ReadInt32( bb )
        o.account_id = ReadInt32( bb )
        o.enter_time = ReadInt64( bb )
        o.enter_money = ReadDouble( bb )
        o.leave_time = ReadInt64( bb )
        o.leave_money = ReadDouble( bb )
        o.consume = ReadDouble( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteInt64 = bb.WriteInt64
        local WriteDouble = bb.WriteDouble
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.level_id )
        WriteInt32( bb, o.room_id )
        WriteInt32( bb, o.account_id )
        WriteInt64( bb, o.enter_time )
        WriteDouble( bb, o.enter_money )
        WriteInt64( bb, o.leave_time )
        WriteDouble( bb, o.leave_money )
        WriteDouble( bb, o.consume )
    end
}
BBuffer.Register( PKG_Tables_log_game_catchfish )
--[[
捕鱼玩家事件日志基表
]]
PKG_Tables_log_game_catchfish_event = {
    typeName = "PKG_Tables_log_game_catchfish_event",
    typeId = 43,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game_catchfish_event
        o.__index = o
        o.__newindex = o

        --[[
        由游戏服自己生成的自增id. 用于派生表关联
        ]]
        o.id = 0 -- Int32
        --[[
        所属对局记录id. 用于派生表关联
        ]]
        o.log_game_catchfish_id = 0 -- Int32
        --[[
        事件类型编号. 1: fire   2: fishdie   3: bulletdie    4: bulletmiss
        ]]
        o.event_type_id = 0 -- Int32
        --[[
        发生时的帧编号
        ]]
        o.frame_number = 0 -- Int32
        --[[
        发生时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.log_game_catchfish_id = ReadInt32( bb )
        o.event_type_id = ReadInt32( bb )
        o.frame_number = ReadInt32( bb )
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.log_game_catchfish_id )
        WriteInt32( bb, o.event_type_id )
        WriteInt32( bb, o.frame_number )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_game_catchfish_event )
--[[
捕鱼玩家事件日志之子弹没打死鱼, 消耗掉了(bulletdie)
]]
PKG_Tables_log_game_catchfish_event_bulletdie = {
    typeName = "PKG_Tables_log_game_catchfish_event_bulletdie",
    typeId = 44,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game_catchfish_event_bulletdie
        o.__index = o
        o.__newindex = o

        --[[
        关联到基表
        ]]
        o.id = 0 -- Int32
        --[[
        关联到基表
        ]]
        o.log_game_catchfish_id = 0 -- Int32
        --[[
        所属类型编号. 0: 普通子弹    1: 钻头蟹    2: 炸弹蟹    3: 闪电鲨
        ]]
        o.type_id = 0 -- Int32
        --[[
        子弹序号
        ]]
        o.bullet_id = 0 -- Int32
        --[[
        子弹金币数/倍率/强度
        ]]
        o.coin = 0 -- Int64
        --[[
        折算成 account.money 相同货币单位的金额
        ]]
        o.money = 0 -- Double
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.log_game_catchfish_id = ReadInt32( bb )
        o.type_id = ReadInt32( bb )
        o.bullet_id = ReadInt32( bb )
        o.coin = bb:ReadInt64()
        o.money = bb:ReadDouble()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.log_game_catchfish_id )
        WriteInt32( bb, o.type_id )
        WriteInt32( bb, o.bullet_id )
        bb:WriteInt64( o.coin )
        bb:WriteDouble( o.money )
    end
}
BBuffer.Register( PKG_Tables_log_game_catchfish_event_bulletdie )
--[[
捕鱼玩家事件日志之子弹打空, 得返还(bulletmiss)
]]
PKG_Tables_log_game_catchfish_event_bulletmiss = {
    typeName = "PKG_Tables_log_game_catchfish_event_bulletmiss",
    typeId = 45,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game_catchfish_event_bulletmiss
        o.__index = o
        o.__newindex = o

        --[[
        关联到基表
        ]]
        o.id = 0 -- Int32
        --[[
        关联到基表
        ]]
        o.log_game_catchfish_id = 0 -- Int32
        --[[
        所属类型编号. 0: 普通子弹    1: 钻头蟹    2: 炸弹蟹    3: 闪电鲨
        ]]
        o.type_id = 0 -- Int32
        --[[
        子弹序号
        ]]
        o.bullet_id = 0 -- Int32
        --[[
        子弹金币数/倍率/强度
        ]]
        o.coin = 0 -- Int64
        --[[
        折算成 account.money 相同货币单位的金额
        ]]
        o.money = 0 -- Double
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.log_game_catchfish_id = ReadInt32( bb )
        o.type_id = ReadInt32( bb )
        o.bullet_id = ReadInt32( bb )
        o.coin = bb:ReadInt64()
        o.money = bb:ReadDouble()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.log_game_catchfish_id )
        WriteInt32( bb, o.type_id )
        WriteInt32( bb, o.bullet_id )
        bb:WriteInt64( o.coin )
        bb:WriteDouble( o.money )
    end
}
BBuffer.Register( PKG_Tables_log_game_catchfish_event_bulletmiss )
--[[
捕鱼玩家事件日志之开火(fire) 注意: 非普通子弹属于服务器强制开火
]]
PKG_Tables_log_game_catchfish_event_fire = {
    typeName = "PKG_Tables_log_game_catchfish_event_fire",
    typeId = 46,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game_catchfish_event_fire
        o.__index = o
        o.__newindex = o

        --[[
        关联到基表
        ]]
        o.id = 0 -- Int32
        --[[
        关联到基表
        ]]
        o.log_game_catchfish_id = 0 -- Int32
        --[[
        所属类型编号. 0: 普通子弹    1: 钻头蟹    2: 炸弹蟹    3: 闪电鲨
        ]]
        o.type_id = 0 -- Int32
        --[[
        子弹序号
        ]]
        o.bullet_id = 0 -- Int32
        --[[
        子弹金币数/倍率/强度( 爆炸等效果大批量剩余子弹可合并填写, miss 表也对应合并 )
        ]]
        o.coin = 0 -- Int64
        --[[
        折算成 account.money 相同货币单位的金额
        ]]
        o.money = 0 -- Double
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.log_game_catchfish_id = ReadInt32( bb )
        o.type_id = ReadInt32( bb )
        o.bullet_id = ReadInt32( bb )
        o.coin = bb:ReadInt64()
        o.money = bb:ReadDouble()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.log_game_catchfish_id )
        WriteInt32( bb, o.type_id )
        WriteInt32( bb, o.bullet_id )
        bb:WriteInt64( o.coin )
        bb:WriteDouble( o.money )
    end
}
BBuffer.Register( PKG_Tables_log_game_catchfish_event_fire )
--[[
捕鱼玩家事件日志之打死鱼得到钱
]]
PKG_Tables_log_game_catchfish_event_fishdie = {
    typeName = "PKG_Tables_log_game_catchfish_event_fishdie",
    typeId = 47,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game_catchfish_event_fishdie
        o.__index = o
        o.__newindex = o

        --[[
        关联到基表
        ]]
        o.id = 0 -- Int32
        --[[
        关联到基表
        ]]
        o.log_game_catchfish_id = 0 -- Int32
        --[[
        鱼类型/配置编号. miss 情况下为 -1
        ]]
        o.fish_type_id = 0 -- Int32
        --[[
        鱼内部编号. miss 情况下为 0
        ]]
        o.fish_id = 0 -- Int32
        --[[
        鱼倍率. miss 情况下为 1
        ]]
        o.fish_ratio = 0 -- Int64
        --[[
        子弹金币数
        ]]
        o.bullet_coin = 0 -- Int64
        --[[
        最终玩家所得 = 子弹金币数 * 鱼倍率
        ]]
        o.coin = 0 -- Int64
        --[[
        最终玩家所得 折算成 account.money 相同货币单位的金额
        ]]
        o.money = 0 -- Double
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadInt64 = bb.ReadInt64
        o.id = ReadInt32( bb )
        o.log_game_catchfish_id = ReadInt32( bb )
        o.fish_type_id = ReadInt32( bb )
        o.fish_id = ReadInt32( bb )
        o.fish_ratio = ReadInt64( bb )
        o.bullet_coin = ReadInt64( bb )
        o.coin = ReadInt64( bb )
        o.money = bb:ReadDouble()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteInt64 = bb.WriteInt64
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.log_game_catchfish_id )
        WriteInt32( bb, o.fish_type_id )
        WriteInt32( bb, o.fish_id )
        WriteInt64( bb, o.fish_ratio )
        WriteInt64( bb, o.bullet_coin )
        WriteInt64( bb, o.coin )
        bb:WriteDouble( o.money )
    end
}
BBuffer.Register( PKG_Tables_log_game_catchfish_event_fishdie )
--[[
记录玩家上下线情况的日志. 分4种情况:   1. 上线     2. 断线     3. 重连.    4. 下线
]]
PKG_Tables_log_login_out = {
    typeName = "PKG_Tables_log_login_out",
    typeId = 48,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_login_out
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        --[[
        帐号id
        ]]
        o.account_id = 0 -- Int32
        --[[
        1. 上线     2. 断线     3. 重连.    4. 下线
        ]]
        o.type_id = 0 -- Int32
        --[[
        上线或重连时的IP地址
        ]]
        o.ip = null -- String
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.id = ReadInt32( bb )
        o.account_id = ReadInt32( bb )
        o.type_id = ReadInt32( bb )
        o.ip = bb:ReadObject()
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.account_id )
        WriteInt32( bb, o.type_id )
        bb:WriteObject( o.ip )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_login_out )
--[[
申请提现状态修改日志表
]]
PKG_Tables_log_withdrawals = {
    typeName = "PKG_Tables_log_withdrawals",
    typeId = 49,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_withdrawals
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        --[[
        申请提现表id
        ]]
        o.withdrawals_id = 0 -- Int32
        --[[
        操作人(客服)
        ]]
        o.person_in_charge = null -- String
        --[[
        操作状态
        ]]
        o.state_id = 0 -- Int32
        --[[
        操作描述
        ]]
        o.description = null -- String
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadObject = bb.ReadObject
        o.id = ReadInt32( bb )
        o.withdrawals_id = ReadInt32( bb )
        o.person_in_charge = ReadObject( bb )
        o.state_id = ReadInt32( bb )
        o.description = ReadObject( bb )
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteObject = bb.WriteObject
        WriteInt32( bb, o.id )
        WriteInt32( bb, o.withdrawals_id )
        WriteObject( bb, o.person_in_charge )
        WriteInt32( bb, o.state_id )
        WriteObject( bb, o.description )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_withdrawals )
--[[
邮箱表
]]
PKG_Tables_mail = {
    typeName = "PKG_Tables_mail",
    typeId = 50,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_mail
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        --[[
        内容
        ]]
        o.content = null -- String
        --[[
        收件人
        ]]
        o.account_id = 0 -- Int32
        --[[
        发件人
        ]]
        o.sender = null -- String
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        --[[
        是否已读(0未读, 1已读)
        ]]
        o.is_read = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadObject = bb.ReadObject
        o.id = ReadInt32( bb )
        o.content = ReadObject( bb )
        o.account_id = ReadInt32( bb )
        o.sender = ReadObject( bb )
        o.create_time = bb:ReadInt64()
        o.is_read = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteObject = bb.WriteObject
        WriteInt32( bb, o.id )
        WriteObject( bb, o.content )
        WriteInt32( bb, o.account_id )
        WriteObject( bb, o.sender )
        bb:WriteInt64( o.create_time )
        WriteInt32( bb, o.is_read )
    end
}
BBuffer.Register( PKG_Tables_mail )
--[[
跑马灯表
]]
PKG_Tables_marquee = {
    typeName = "PKG_Tables_marquee",
    typeId = 51,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_marquee
        o.__index = o
        o.__newindex = o

        --[[
        自增id
        ]]
        o.id = 0 -- Int32
        --[[
        内容
        ]]
        o.content = null -- String
        --[[
        喊话频率(多长时间循环执行一次，单位 秒)
        ]]
        o.frequency = 0 -- Int32
        --[[
        更新时间
        ]]
        o.update_time = 0 -- Int64
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        --[[
        是否有效(1有效)
        ]]
        o.enable = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadInt64 = bb.ReadInt64
        o.id = ReadInt32( bb )
        o.content = bb:ReadObject()
        o.frequency = ReadInt32( bb )
        o.update_time = ReadInt64( bb )
        o.create_time = ReadInt64( bb )
        o.enable = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteInt64 = bb.WriteInt64
        WriteInt32( bb, o.id )
        bb:WriteObject( o.content )
        WriteInt32( bb, o.frequency )
        WriteInt64( bb, o.update_time )
        WriteInt64( bb, o.create_time )
        WriteInt32( bb, o.enable )
    end
}
BBuffer.Register( PKG_Tables_marquee )
--[[
公告表
]]
PKG_Tables_notice = {
    typeName = "PKG_Tables_notice",
    typeId = 52,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_notice
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        --[[
        公告内容
        ]]
        o.content = null -- String
        --[[
        公告类型
        ]]
        o.notice_type_id = 0 -- Int32
        --[[
        有效起始时间
        ]]
        o.start_time = null -- NullableInt64
        --[[
        有效结束时间
        ]]
        o.end_time = null -- NullableInt64
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadObject = bb.ReadObject
        o.id = ReadInt32( bb )
        o.content = ReadObject( bb )
        o.notice_type_id = ReadInt32( bb )
        o.start_time = ReadObject( bb )
        o.end_time = ReadObject( bb )
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteObject = bb.WriteObject
        WriteInt32( bb, o.id )
        WriteObject( bb, o.content )
        WriteInt32( bb, o.notice_type_id )
        WriteObject( bb, o.start_time )
        WriteObject( bb, o.end_time )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_notice )
--[[
公告读取表
]]
PKG_Tables_notice_read = {
    typeName = "PKG_Tables_notice_read",
    typeId = 53,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_notice_read
        o.__index = o
        o.__newindex = o

        --[[
        公告id
        ]]
        o.notice_id = 0 -- Int32
        --[[
        玩家id
        ]]
        o.account_id = 0 -- Int32
        --[[
        创建时间
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.notice_id = ReadInt32( bb )
        o.account_id = ReadInt32( bb )
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.notice_id )
        WriteInt32( bb, o.account_id )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_notice_read )
--[[
公告类型表
]]
PKG_Tables_notice_type = {
    typeName = "PKG_Tables_notice_type",
    typeId = 54,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_notice_type
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        o.name = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt32()
        o.name = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.id )
        bb:WriteObject( o.name )
    end
}
BBuffer.Register( PKG_Tables_notice_type )
--[[
短信校验状态数据( 数据有有效期. 数据服定时删掉超时数据 )
]]
PKG_Tables_verify_code = {
    typeName = "PKG_Tables_verify_code",
    typeId = 55,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_verify_code
        o.__index = o
        o.__newindex = o

        --[[
        收短信的电话号码
        ]]
        o.phone = null -- String
        --[[
        短信验证码内容
        ]]
        o.content = null -- String
        --[[
        下发时间( 用于判断数据是否过期, 过期就删除 )
        ]]
        o.create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.phone = ReadObject( bb )
        o.content = ReadObject( bb )
        o.create_time = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.phone )
        WriteObject( bb, o.content )
        bb:WriteInt64( o.create_time )
    end
}
BBuffer.Register( PKG_Tables_verify_code )
--[[
VIP与累计充值金额的对应表
]]
PKG_Tables_vip = {
    typeName = "PKG_Tables_vip",
    typeId = 56,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_vip
        o.__index = o
        o.__newindex = o

        --[[
        唯一编号 & 级别
        ]]
        o.id = 0 -- Int32
        --[[
        累计充值金额要求
        ]]
        o.total_recharge = 0 -- Double
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt32()
        o.total_recharge = bb:ReadDouble()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.id )
        bb:WriteDouble( o.total_recharge )
    end
}
BBuffer.Register( PKG_Tables_vip )
--[[
申请提现表
]]
PKG_Tables_withdrawals = {
    typeName = "PKG_Tables_withdrawals",
    typeId = 57,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_withdrawals
        o.__index = o
        o.__newindex = o

        --[[
        提现订单号( 随机 9 位整数 )
        ]]
        o.withdrawals_id = 0 -- Int32
        --[[
        帐号id
        ]]
        o.account_id = 0 -- Int32
        --[[
        提现金额
        ]]
        o.money = 0 -- Double
        --[[
        提现请求( 渠道, 账号等 )
        ]]
        o.requirement = null -- String
        --[[
        操作状态
        ]]
        o.state_id = 0 -- Int32
        --[[
        操作描述
        ]]
        o.description = null -- String
        --[[
        提现申请时间
        ]]
        o.create_time = 0 -- Int64
        --[[
        最后处理时间
        ]]
        o.last_create_time = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadObject = bb.ReadObject
        local ReadInt64 = bb.ReadInt64
        o.withdrawals_id = ReadInt32( bb )
        o.account_id = ReadInt32( bb )
        o.money = bb:ReadDouble()
        o.requirement = ReadObject( bb )
        o.state_id = ReadInt32( bb )
        o.description = ReadObject( bb )
        o.create_time = ReadInt64( bb )
        o.last_create_time = ReadInt64( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteObject = bb.WriteObject
        local WriteInt64 = bb.WriteInt64
        WriteInt32( bb, o.withdrawals_id )
        WriteInt32( bb, o.account_id )
        bb:WriteDouble( o.money )
        WriteObject( bb, o.requirement )
        WriteInt32( bb, o.state_id )
        WriteObject( bb, o.description )
        WriteInt64( bb, o.create_time )
        WriteInt64( bb, o.last_create_time )
    end
}
BBuffer.Register( PKG_Tables_withdrawals )
--[[
提现状态表
]]
PKG_Tables_withdrawals_state = {
    typeName = "PKG_Tables_withdrawals_state",
    typeId = 58,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_withdrawals_state
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        o.name = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt32()
        o.name = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.id )
        bb:WriteObject( o.name )
    end
}
BBuffer.Register( PKG_Tables_withdrawals_state )