
PKG_PkgGenMd5_Value = 'bf0a438febf3b576bbe0485476196fd2'

PKG_Foo = {
    typeName = "PKG_Foo",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = PKG_Foo
        o.__index = o
        o.__newindex = o

        o.foo = null -- PKG_Foo
        o.refFoo = null -- Ref
        o.foos = null -- List_PKG_Foo_
        o.refFoos = null -- List_Ref_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.foo = ReadObject( bb )
        o.refFoo = ReadObject( bb )
        o.foos = ReadObject( bb )
        o.refFoos = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.foo )
        WriteObject( bb, o.refFoo )
        WriteObject( bb, o.foos )
        WriteObject( bb, o.refFoos )
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
List_Ref_ = {
    typeName = "List_Ref_",
    typeId = 5,
    Create = function()
        local o = {}
        o.__proto = List_Ref_
        o.__index = o
        o.__newindex = o
        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
        local f = BBuffer.ReadRef
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteRef
        for i = 1, len do
			f( bb, o[ i ] )
		end
    end
}
BBuffer.Register( List_Ref_ )