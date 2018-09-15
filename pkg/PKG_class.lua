
PKG_PkgGenMd5_Value = 'c4dfb0ecd26efc3cdfa643f478b61473'

PKG_Foo = {
    typeName = "PKG_Foo",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = PKG_Foo
        o.__index = o
        o.__newindex = o

        o.foo = null -- PKG_Foo
        o.foos = null -- List_PKG_Foo_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.foo = ReadObject( bb )
        o.foos = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.foo )
        WriteObject( bb, o.foos )
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