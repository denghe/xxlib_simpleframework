
PKG_PkgGenMd5_Value = 'cbccc319ed09be93c69184ff1bf22cc9'

PKG_Player = {
    typeName = "PKG_Player",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = PKG_Player
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.id = 0 -- Int32
        o.name = null -- String
        o.owner = null -- PKG_Scene
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = bb:ReadInt32()
        o.name = ReadObject( bb )
        o.owner = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        bb:WriteInt32( o.id )
        WriteObject( bb, o.name )
        WriteObject( bb, o.owner )
    end
}
BBuffer.Register( PKG_Player )
PKG_Scene = {
    typeName = "PKG_Scene",
    typeId = 4,
    Create = function()
        local o = {}
        o.__proto = PKG_Scene
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.players = null -- List_Ref_PKG_Player_
        return o
    end,
    FromBBuffer = function( bb, o )
        o.players = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.players )
    end
}
BBuffer.Register( PKG_Scene )
List_Ref_PKG_Player_ = {
    typeName = "List_Ref_PKG_Player_",
    typeId = 5,
    Create = function()
        local o = {}
        o.__proto = List_Ref_PKG_Player_
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
BBuffer.Register( List_Ref_PKG_Player_ )