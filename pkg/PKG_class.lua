
PKG_PkgGenMd5_Value = '267d82049bdda2a37af33059b40eaf53'

PKG_Container = {
    typeName = "PKG_Container",
    typeId = 7,
    Create = function()
        local o = {}
        o.__proto = PKG_Container
        o.__index = o
        o.__newindex = o

        o.foos = null -- List_PKG_Foo_
        return o
    end,
    FromBBuffer = function( bb, o )
        o.foos = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.foos )
    end
}
BBuffer.Register( PKG_Container )
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
PKG_Foo = {
    typeName = "PKG_Foo",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = PKG_Foo
        o.__index = o
        o.__newindex = o

        o.childs = null -- List_PKG_Foo_
        return o
    end,
    FromBBuffer = function( bb, o )
        o.childs = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.childs )
    end
}
BBuffer.Register( PKG_Foo )
PKG_FooEx = {
    typeName = "PKG_FooEx",
    typeId = 6,
    Create = function()
        local o = {}
        o.__proto = PKG_FooEx
        o.__index = o
        o.__newindex = o

        o.parent = null -- PKG_Foo
        setmetatable( o, PKG_Foo.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.parent = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteObject( o.parent )
    end
}
BBuffer.Register( PKG_FooEx )