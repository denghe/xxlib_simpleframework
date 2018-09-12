
PKG_PkgGenMd5_Value = 'bd83cd63ba80d8ea0f7f8fb9501ab20c'

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