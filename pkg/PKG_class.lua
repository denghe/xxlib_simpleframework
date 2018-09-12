
PKG_PkgGenMd5_Value = 'a6a6b3c8d511000a913006017240448c'

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
PKG_Tables_account = {
    typeName = "PKG_Tables_account",
    typeId = 22,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_account
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.username = null -- String
        o.nickname = null -- String
        o.avatar_id = null -- String
        o.phone = null -- String
        o.password = null -- String
        o.money = null -- String
        o.money_safe = null -- String
        o.total_recharge = null -- String
        o.total_consume = null -- String
        o.total_withdraw = null -- String
        o.create_time = null -- String
        o.enabled = null -- String
        o.enter_lobby_time = null -- String
        o.log_game_id = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.username = ReadObject( bb )
        o.nickname = ReadObject( bb )
        o.avatar_id = ReadObject( bb )
        o.phone = ReadObject( bb )
        o.password = ReadObject( bb )
        o.money = ReadObject( bb )
        o.money_safe = ReadObject( bb )
        o.total_recharge = ReadObject( bb )
        o.total_consume = ReadObject( bb )
        o.total_withdraw = ReadObject( bb )
        o.create_time = ReadObject( bb )
        o.enabled = ReadObject( bb )
        o.enter_lobby_time = ReadObject( bb )
        o.log_game_id = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.username )
        WriteObject( bb, o.nickname )
        WriteObject( bb, o.avatar_id )
        WriteObject( bb, o.phone )
        WriteObject( bb, o.password )
        WriteObject( bb, o.money )
        WriteObject( bb, o.money_safe )
        WriteObject( bb, o.total_recharge )
        WriteObject( bb, o.total_consume )
        WriteObject( bb, o.total_withdraw )
        WriteObject( bb, o.create_time )
        WriteObject( bb, o.enabled )
        WriteObject( bb, o.enter_lobby_time )
        WriteObject( bb, o.log_game_id )
    end
}
BBuffer.Register( PKG_Tables_account )
PKG_Tables_chat = {
    typeName = "PKG_Tables_chat",
    typeId = 23,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_chat
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.content = null -- String
        o.account_id = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.content = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.content )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_chat )
PKG_Tables_config = {
    typeName = "PKG_Tables_config",
    typeId = 24,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_config
        o.__index = o
        o.__newindex = o

        o.key = null -- String
        o.datatype_name = null -- String
        o.title = null -- String
        o.desc = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.key = ReadObject( bb )
        o.datatype_name = ReadObject( bb )
        o.title = ReadObject( bb )
        o.desc = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.key )
        WriteObject( bb, o.datatype_name )
        WriteObject( bb, o.title )
        WriteObject( bb, o.desc )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_config )
PKG_Tables_config_value = {
    typeName = "PKG_Tables_config_value",
    typeId = 25,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_config_value
        o.__index = o
        o.__newindex = o

        o.key = null -- String
        o.value_bigint = null -- String
        o.value_double = null -- String
        o.value_varchar = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.key = ReadObject( bb )
        o.value_bigint = ReadObject( bb )
        o.value_double = ReadObject( bb )
        o.value_varchar = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.key )
        WriteObject( bb, o.value_bigint )
        WriteObject( bb, o.value_double )
        WriteObject( bb, o.value_varchar )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_config_value )
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
PKG_Tables_fish = {
    typeName = "PKG_Tables_fish",
    typeId = 27,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_fish
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.name = null -- String
        o.minCoin = null -- String
        o.maxCoin = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.name = ReadObject( bb )
        o.minCoin = ReadObject( bb )
        o.maxCoin = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.name )
        WriteObject( bb, o.minCoin )
        WriteObject( bb, o.maxCoin )
    end
}
BBuffer.Register( PKG_Tables_fish )
PKG_Tables_game = {
    typeName = "PKG_Tables_game",
    typeId = 28,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_game
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.name = null -- String
        o.type_name = null -- String
        o.enabled = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.name = ReadObject( bb )
        o.type_name = ReadObject( bb )
        o.enabled = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.name )
        WriteObject( bb, o.type_name )
        WriteObject( bb, o.enabled )
    end
}
BBuffer.Register( PKG_Tables_game )
PKG_Tables_game_catchfish = {
    typeName = "PKG_Tables_game_catchfish",
    typeId = 29,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_game_catchfish
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.exchange_coin_ratio = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.exchange_coin_ratio = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.exchange_coin_ratio )
    end
}
BBuffer.Register( PKG_Tables_game_catchfish )
PKG_Tables_game_catchfish_account = {
    typeName = "PKG_Tables_game_catchfish_account",
    typeId = 30,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_game_catchfish_account
        o.__index = o
        o.__newindex = o

        o.account_id = null -- String
        o.game_id = null -- String
        o.level_id = null -- String
        o.last_bet = null -- String
        o.last_cannon_id = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.account_id = ReadObject( bb )
        o.game_id = ReadObject( bb )
        o.level_id = ReadObject( bb )
        o.last_bet = ReadObject( bb )
        o.last_cannon_id = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.game_id )
        WriteObject( bb, o.level_id )
        WriteObject( bb, o.last_bet )
        WriteObject( bb, o.last_cannon_id )
    end
}
BBuffer.Register( PKG_Tables_game_catchfish_account )
PKG_Tables_game_catchfish_cannon = {
    typeName = "PKG_Tables_game_catchfish_cannon",
    typeId = 31,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_game_catchfish_cannon
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.vip_id = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.vip_id = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.vip_id )
    end
}
BBuffer.Register( PKG_Tables_game_catchfish_cannon )
PKG_Tables_game_catchfish_level = {
    typeName = "PKG_Tables_game_catchfish_level",
    typeId = 32,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_game_catchfish_level
        o.__index = o
        o.__newindex = o

        o.game_id = null -- String
        o.level_id = null -- String
        o.min_bet = null -- String
        o.max_bet = null -- String
        o.min_money = null -- String
        o.enabled = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.game_id = ReadObject( bb )
        o.level_id = ReadObject( bb )
        o.min_bet = ReadObject( bb )
        o.max_bet = ReadObject( bb )
        o.min_money = ReadObject( bb )
        o.enabled = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.game_id )
        WriteObject( bb, o.level_id )
        WriteObject( bb, o.min_bet )
        WriteObject( bb, o.max_bet )
        WriteObject( bb, o.min_money )
        WriteObject( bb, o.enabled )
    end
}
BBuffer.Register( PKG_Tables_game_catchfish_level )
PKG_Tables_game_catchfish_level_room = {
    typeName = "PKG_Tables_game_catchfish_level_room",
    typeId = 33,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_game_catchfish_level_room
        o.__index = o
        o.__newindex = o

        o.game_id = null -- String
        o.level_id = null -- String
        o.room_id = null -- String
        o.enabled = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.game_id = ReadObject( bb )
        o.level_id = ReadObject( bb )
        o.room_id = ReadObject( bb )
        o.enabled = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.game_id )
        WriteObject( bb, o.level_id )
        WriteObject( bb, o.room_id )
        WriteObject( bb, o.enabled )
    end
}
BBuffer.Register( PKG_Tables_game_catchfish_level_room )
PKG_Tables_log_account_avatar = {
    typeName = "PKG_Tables_log_account_avatar",
    typeId = 34,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_avatar
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.account_id = null -- String
        o.avatar_id = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.avatar_id = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.avatar_id )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_avatar )
PKG_Tables_log_account_enabled = {
    typeName = "PKG_Tables_log_account_enabled",
    typeId = 35,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_enabled
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.account_id = null -- String
        o.enabled = null -- String
        o.memo = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.enabled = ReadObject( bb )
        o.memo = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.enabled )
        WriteObject( bb, o.memo )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_enabled )
PKG_Tables_log_account_money_safe = {
    typeName = "PKG_Tables_log_account_money_safe",
    typeId = 36,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_money_safe
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.account_id = null -- String
        o.value = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.value = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.value )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_money_safe )
PKG_Tables_log_account_nickname = {
    typeName = "PKG_Tables_log_account_nickname",
    typeId = 37,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_nickname
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.account_id = null -- String
        o.nickname = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.nickname = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.nickname )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_nickname )
PKG_Tables_log_account_password = {
    typeName = "PKG_Tables_log_account_password",
    typeId = 38,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_password
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.account_id = null -- String
        o.password = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.password = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.password )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_password )
PKG_Tables_log_account_phone = {
    typeName = "PKG_Tables_log_account_phone",
    typeId = 39,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_phone
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.account_id = null -- String
        o.phone = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.phone = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.phone )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_phone )
PKG_Tables_log_account_recharge = {
    typeName = "PKG_Tables_log_account_recharge",
    typeId = 40,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_account_recharge
        o.__index = o
        o.__newindex = o

        o.token = null -- String
        o.account_id = null -- String
        o.money = null -- String
        o.memo = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.token = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.money = ReadObject( bb )
        o.memo = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.token )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.money )
        WriteObject( bb, o.memo )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_account_recharge )
PKG_Tables_log_game = {
    typeName = "PKG_Tables_log_game",
    typeId = 41,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.game_id = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.game_id = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.game_id )
    end
}
BBuffer.Register( PKG_Tables_log_game )
PKG_Tables_log_game_catchfish = {
    typeName = "PKG_Tables_log_game_catchfish",
    typeId = 42,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game_catchfish
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.level_id = null -- String
        o.room_id = null -- String
        o.account_id = null -- String
        o.enter_time = null -- String
        o.enter_money = null -- String
        o.leave_time = null -- String
        o.leave_money = null -- String
        o.consume = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.level_id = ReadObject( bb )
        o.room_id = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.enter_time = ReadObject( bb )
        o.enter_money = ReadObject( bb )
        o.leave_time = ReadObject( bb )
        o.leave_money = ReadObject( bb )
        o.consume = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.level_id )
        WriteObject( bb, o.room_id )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.enter_time )
        WriteObject( bb, o.enter_money )
        WriteObject( bb, o.leave_time )
        WriteObject( bb, o.leave_money )
        WriteObject( bb, o.consume )
    end
}
BBuffer.Register( PKG_Tables_log_game_catchfish )
PKG_Tables_log_game_catchfish_event = {
    typeName = "PKG_Tables_log_game_catchfish_event",
    typeId = 43,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game_catchfish_event
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.log_game_catchfish_id = null -- String
        o.event_type_id = null -- String
        o.frame_number = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.log_game_catchfish_id = ReadObject( bb )
        o.event_type_id = ReadObject( bb )
        o.frame_number = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.log_game_catchfish_id )
        WriteObject( bb, o.event_type_id )
        WriteObject( bb, o.frame_number )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_game_catchfish_event )
PKG_Tables_log_game_catchfish_event_bulletdie = {
    typeName = "PKG_Tables_log_game_catchfish_event_bulletdie",
    typeId = 44,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game_catchfish_event_bulletdie
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.log_game_catchfish_id = null -- String
        o.type_id = null -- String
        o.bullet_id = null -- String
        o.coin = null -- String
        o.money = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.log_game_catchfish_id = ReadObject( bb )
        o.type_id = ReadObject( bb )
        o.bullet_id = ReadObject( bb )
        o.coin = ReadObject( bb )
        o.money = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.log_game_catchfish_id )
        WriteObject( bb, o.type_id )
        WriteObject( bb, o.bullet_id )
        WriteObject( bb, o.coin )
        WriteObject( bb, o.money )
    end
}
BBuffer.Register( PKG_Tables_log_game_catchfish_event_bulletdie )
PKG_Tables_log_game_catchfish_event_bulletmiss = {
    typeName = "PKG_Tables_log_game_catchfish_event_bulletmiss",
    typeId = 45,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game_catchfish_event_bulletmiss
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.log_game_catchfish_id = null -- String
        o.type_id = null -- String
        o.bullet_id = null -- String
        o.coin = null -- String
        o.money = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.log_game_catchfish_id = ReadObject( bb )
        o.type_id = ReadObject( bb )
        o.bullet_id = ReadObject( bb )
        o.coin = ReadObject( bb )
        o.money = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.log_game_catchfish_id )
        WriteObject( bb, o.type_id )
        WriteObject( bb, o.bullet_id )
        WriteObject( bb, o.coin )
        WriteObject( bb, o.money )
    end
}
BBuffer.Register( PKG_Tables_log_game_catchfish_event_bulletmiss )
PKG_Tables_log_game_catchfish_event_fire = {
    typeName = "PKG_Tables_log_game_catchfish_event_fire",
    typeId = 46,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game_catchfish_event_fire
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.log_game_catchfish_id = null -- String
        o.type_id = null -- String
        o.bullet_id = null -- String
        o.coin = null -- String
        o.money = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.log_game_catchfish_id = ReadObject( bb )
        o.type_id = ReadObject( bb )
        o.bullet_id = ReadObject( bb )
        o.coin = ReadObject( bb )
        o.money = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.log_game_catchfish_id )
        WriteObject( bb, o.type_id )
        WriteObject( bb, o.bullet_id )
        WriteObject( bb, o.coin )
        WriteObject( bb, o.money )
    end
}
BBuffer.Register( PKG_Tables_log_game_catchfish_event_fire )
PKG_Tables_log_game_catchfish_event_fishdie = {
    typeName = "PKG_Tables_log_game_catchfish_event_fishdie",
    typeId = 47,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_game_catchfish_event_fishdie
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.log_game_catchfish_id = null -- String
        o.fish_type_id = null -- String
        o.fish_id = null -- String
        o.fish_ratio = null -- String
        o.bullet_coin = null -- String
        o.coin = null -- String
        o.money = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.log_game_catchfish_id = ReadObject( bb )
        o.fish_type_id = ReadObject( bb )
        o.fish_id = ReadObject( bb )
        o.fish_ratio = ReadObject( bb )
        o.bullet_coin = ReadObject( bb )
        o.coin = ReadObject( bb )
        o.money = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.log_game_catchfish_id )
        WriteObject( bb, o.fish_type_id )
        WriteObject( bb, o.fish_id )
        WriteObject( bb, o.fish_ratio )
        WriteObject( bb, o.bullet_coin )
        WriteObject( bb, o.coin )
        WriteObject( bb, o.money )
    end
}
BBuffer.Register( PKG_Tables_log_game_catchfish_event_fishdie )
PKG_Tables_log_login_out = {
    typeName = "PKG_Tables_log_login_out",
    typeId = 48,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_login_out
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.account_id = null -- String
        o.type_id = null -- String
        o.ip = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.type_id = ReadObject( bb )
        o.ip = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.type_id )
        WriteObject( bb, o.ip )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_login_out )
PKG_Tables_log_withdrawals = {
    typeName = "PKG_Tables_log_withdrawals",
    typeId = 49,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_log_withdrawals
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.withdrawals_id = null -- String
        o.person_in_charge = null -- String
        o.state_id = null -- String
        o.description = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.withdrawals_id = ReadObject( bb )
        o.person_in_charge = ReadObject( bb )
        o.state_id = ReadObject( bb )
        o.description = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.withdrawals_id )
        WriteObject( bb, o.person_in_charge )
        WriteObject( bb, o.state_id )
        WriteObject( bb, o.description )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_log_withdrawals )
PKG_Tables_mail = {
    typeName = "PKG_Tables_mail",
    typeId = 50,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_mail
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.content = null -- String
        o.account_id = null -- String
        o.sender = null -- String
        o.create_time = null -- String
        o.is_read = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.content = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.sender = ReadObject( bb )
        o.create_time = ReadObject( bb )
        o.is_read = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.content )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.sender )
        WriteObject( bb, o.create_time )
        WriteObject( bb, o.is_read )
    end
}
BBuffer.Register( PKG_Tables_mail )
PKG_Tables_marquee = {
    typeName = "PKG_Tables_marquee",
    typeId = 51,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_marquee
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.content = null -- String
        o.frequency = null -- String
        o.update_time = null -- String
        o.create_time = null -- String
        o.enable = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.content = ReadObject( bb )
        o.frequency = ReadObject( bb )
        o.update_time = ReadObject( bb )
        o.create_time = ReadObject( bb )
        o.enable = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.content )
        WriteObject( bb, o.frequency )
        WriteObject( bb, o.update_time )
        WriteObject( bb, o.create_time )
        WriteObject( bb, o.enable )
    end
}
BBuffer.Register( PKG_Tables_marquee )
PKG_Tables_notice = {
    typeName = "PKG_Tables_notice",
    typeId = 52,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_notice
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.content = null -- String
        o.notice_type_id = null -- String
        o.start_time = null -- String
        o.end_time = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.content = ReadObject( bb )
        o.notice_type_id = ReadObject( bb )
        o.start_time = ReadObject( bb )
        o.end_time = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.content )
        WriteObject( bb, o.notice_type_id )
        WriteObject( bb, o.start_time )
        WriteObject( bb, o.end_time )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_notice )
PKG_Tables_notice_read = {
    typeName = "PKG_Tables_notice_read",
    typeId = 53,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_notice_read
        o.__index = o
        o.__newindex = o

        o.notice_id = null -- String
        o.account_id = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.notice_id = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.notice_id )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_notice_read )
PKG_Tables_notice_type = {
    typeName = "PKG_Tables_notice_type",
    typeId = 54,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_notice_type
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.name = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.name = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.name )
    end
}
BBuffer.Register( PKG_Tables_notice_type )
PKG_Tables_verify_code = {
    typeName = "PKG_Tables_verify_code",
    typeId = 55,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_verify_code
        o.__index = o
        o.__newindex = o

        o.phone = null -- String
        o.content = null -- String
        o.create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.phone = ReadObject( bb )
        o.content = ReadObject( bb )
        o.create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.phone )
        WriteObject( bb, o.content )
        WriteObject( bb, o.create_time )
    end
}
BBuffer.Register( PKG_Tables_verify_code )
PKG_Tables_vip = {
    typeName = "PKG_Tables_vip",
    typeId = 56,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_vip
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.total_recharge = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.total_recharge = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.total_recharge )
    end
}
BBuffer.Register( PKG_Tables_vip )
PKG_Tables_withdrawals = {
    typeName = "PKG_Tables_withdrawals",
    typeId = 57,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_withdrawals
        o.__index = o
        o.__newindex = o

        o.withdrawals_id = null -- String
        o.account_id = null -- String
        o.money = null -- String
        o.requirement = null -- String
        o.state_id = null -- String
        o.description = null -- String
        o.create_time = null -- String
        o.last_create_time = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.withdrawals_id = ReadObject( bb )
        o.account_id = ReadObject( bb )
        o.money = ReadObject( bb )
        o.requirement = ReadObject( bb )
        o.state_id = ReadObject( bb )
        o.description = ReadObject( bb )
        o.create_time = ReadObject( bb )
        o.last_create_time = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.withdrawals_id )
        WriteObject( bb, o.account_id )
        WriteObject( bb, o.money )
        WriteObject( bb, o.requirement )
        WriteObject( bb, o.state_id )
        WriteObject( bb, o.description )
        WriteObject( bb, o.create_time )
        WriteObject( bb, o.last_create_time )
    end
}
BBuffer.Register( PKG_Tables_withdrawals )
PKG_Tables_withdrawals_state = {
    typeName = "PKG_Tables_withdrawals_state",
    typeId = 58,
    Create = function()
        local o = {}
        o.__proto = PKG_Tables_withdrawals_state
        o.__index = o
        o.__newindex = o

        o.id = null -- String
        o.name = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.name = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.name )
    end
}
BBuffer.Register( PKG_Tables_withdrawals_state )