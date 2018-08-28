
PKG_PkgGenMd5_Value = 'e28748eae8453c2ad9510602b5ce2828'

PKG_Foo = {
    typeName = "PKG_Foo",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = PKG_Foo
        o.__index = o
        o.__newindex = o

        o.id = 0 -- Int32
        o.age = null -- Nullable
        o.floats = null -- List_Nullable_
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
List_Nullable_ = {
    typeName = "List_Nullable_",
    typeId = 4,
    Create = function()
        local o = {}
        o.__proto = List_Nullable_
        o.__index = o
        o.__newindex = o
        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
        local f = BBuffer.ReadNullable
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
BBuffer.Register( List_Nullable_ )