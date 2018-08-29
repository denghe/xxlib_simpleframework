
PKG_PkgGenMd5_Value = '92ceb88f02b31db6920219f49c173d22'

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
        o.floats = null -- List_NullableSingle_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = bb:ReadInt32()
        o.age = ReadObject( bb )
        o.floats = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        bb:WriteInt32( o.id )
        WriteObject( bb, o.age )
        WriteObject( bb, o.floats )
    end
}
BBuffer.Register( PKG_Foo )
List_NullableSingle_ = {
    typeName = "List_NullableSingle_",
    typeId = 4,
    Create = function()
        local o = {}
        o.__proto = List_NullableSingle_
        o.__index = o
        o.__newindex = o
        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
        local f = BBuffer.ReadNullableSingle
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteNullable
        for i = 1, len do
			f( bb, o[ i ] )
		end
    end
}
BBuffer.Register( List_NullableSingle_ )