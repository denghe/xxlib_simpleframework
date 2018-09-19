
PKG_PkgGenMd5_Value = '72103084919eecc8fb28cf1d5b43999c'

PKG_Foo = {
    typeName = "PKG_Foo",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = PKG_Foo
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.refFoo = MakeRef() -- Ref_PKG_Foo
        o.refFoos = null -- List_Ref_PKG_Foo_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.refFoo = MakeRef( ReadObject( bb ) )
        o.refFoos = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.refFoo.Lock() )
        WriteObject( bb, o.refFoos )
    end
}
BBuffer.Register( PKG_Foo )
List_Ref_PKG_Foo_ = {
    typeName = "List_Ref_PKG_Foo_",
    typeId = 5,
    Create = function()
        local o = {}
        o.__proto = List_Ref_PKG_Foo_
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
			o[ i ] = MakeRef( f( bb ) )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteObject
        for i = 1, len do
			f( bb, o[ i ].Lock() )
		end
    end
}
BBuffer.Register( List_Ref_PKG_Foo_ )