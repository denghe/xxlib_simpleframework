
TMX_PkgGenMd5_Value = '95f3af1091a0990d61078c22f92da1b5'

TMX_OrientationTypes = {
    Unknown = 0,
    Orthogonal = 1,
    Isometric = 2,
    Staggered = 3,
    Hexagonal = 4
}
TMX_RenderOrderTypes = {
    RightDown = 0,
    RightUp = 1,
    LeftDown = 2,
    LeftUp = 3
}
TMX_DrawOrderTypes = {
    Unknown = -1,
    TopDown = 0,
    Index = 1
}
TMX_ObjectTypes = {
    Basic = 0,
    Tile = 1,
    Ellipse = 2,
    Polygon = 3,
    Polyline = 4
}
TMX_Map = {
    typeName = "TMX_Map",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = TMX_Map
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.version = null -- String
        o.tiledversion = null -- String
        o.orientation = 0 -- TMX_OrientationTypes
        o.renderorder = 0 -- TMX_RenderOrderTypes
        o.width = 0 -- Int32
        o.height = 0 -- Int32
        o.tilewidth = 0 -- Int32
        o.tileheight = 0 -- Int32
        o.hexsidelength = null -- NullableInt32
        o.infinite = null -- NullableInt32
        o.nextlayerid = null -- NullableInt32
        o.nextobjectid = null -- NullableInt32
        o.backgroundcolor = null -- TMX_Color4B
        o.tilesets = null -- List_TMX_TileSet_
        o.layers = null -- List_TMX_Layer_
        o.objectgroups = null -- List_TMX_ObjectGroup_
        o.properties = null -- List_TMX_Property_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        local ReadInt32 = bb.ReadInt32
        o.version = ReadObject( bb )
        o.tiledversion = ReadObject( bb )
        o.orientation = ReadInt32( bb )
        o.renderorder = ReadInt32( bb )
        o.width = ReadInt32( bb )
        o.height = ReadInt32( bb )
        o.tilewidth = ReadInt32( bb )
        o.tileheight = ReadInt32( bb )
        o.hexsidelength = ReadObject( bb )
        o.infinite = ReadObject( bb )
        o.nextlayerid = ReadObject( bb )
        o.nextobjectid = ReadObject( bb )
        o.backgroundcolor = ReadObject( bb )
        o.tilesets = ReadObject( bb )
        o.layers = ReadObject( bb )
        o.objectgroups = ReadObject( bb )
        o.properties = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        local WriteInt32 = bb.WriteInt32
        WriteObject( bb, o.version )
        WriteObject( bb, o.tiledversion )
        WriteInt32( bb, o.orientation )
        WriteInt32( bb, o.renderorder )
        WriteInt32( bb, o.width )
        WriteInt32( bb, o.height )
        WriteInt32( bb, o.tilewidth )
        WriteInt32( bb, o.tileheight )
        WriteObject( bb, o.hexsidelength )
        WriteObject( bb, o.infinite )
        WriteObject( bb, o.nextlayerid )
        WriteObject( bb, o.nextobjectid )
        WriteObject( bb, o.backgroundcolor )
        WriteObject( bb, o.tilesets )
        WriteObject( bb, o.layers )
        WriteObject( bb, o.objectgroups )
        WriteObject( bb, o.properties )
    end
}
BBuffer.Register( TMX_Map )
List_TMX_TileSet_ = {
    typeName = "List_TMX_TileSet_",
    typeId = 4,
    Create = function()
        local o = {}
        o.__proto = List_TMX_TileSet_
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
BBuffer.Register( List_TMX_TileSet_ )
TMX_TileSet = {
    typeName = "TMX_TileSet",
    typeId = 5,
    Create = function()
        local o = {}
        o.__proto = TMX_TileSet
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.firstgid = 0 -- Int32
        o.name = null -- String
        o.tilewidth = 0 -- Int32
        o.tileheight = 0 -- Int32
        o.spacing = null -- NullableInt32
        o.margin = null -- NullableInt32
        o.tilecount = null -- NullableInt32
        o.columns = null -- NullableInt32
        o.tileoffsetx = null -- NullableInt32
        o.tileoffsety = null -- NullableInt32
        o.source = null -- String
        o.width = 0 -- Int32
        o.height = 0 -- Int32
        o.trans = null -- TMX_Color4B
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadObject = bb.ReadObject
        o.firstgid = ReadInt32( bb )
        o.name = ReadObject( bb )
        o.tilewidth = ReadInt32( bb )
        o.tileheight = ReadInt32( bb )
        o.spacing = ReadObject( bb )
        o.margin = ReadObject( bb )
        o.tilecount = ReadObject( bb )
        o.columns = ReadObject( bb )
        o.tileoffsetx = ReadObject( bb )
        o.tileoffsety = ReadObject( bb )
        o.source = ReadObject( bb )
        o.width = ReadInt32( bb )
        o.height = ReadInt32( bb )
        o.trans = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteObject = bb.WriteObject
        WriteInt32( bb, o.firstgid )
        WriteObject( bb, o.name )
        WriteInt32( bb, o.tilewidth )
        WriteInt32( bb, o.tileheight )
        WriteObject( bb, o.spacing )
        WriteObject( bb, o.margin )
        WriteObject( bb, o.tilecount )
        WriteObject( bb, o.columns )
        WriteObject( bb, o.tileoffsetx )
        WriteObject( bb, o.tileoffsety )
        WriteObject( bb, o.source )
        WriteInt32( bb, o.width )
        WriteInt32( bb, o.height )
        WriteObject( bb, o.trans )
    end
}
BBuffer.Register( TMX_TileSet )
List_TMX_Layer_ = {
    typeName = "List_TMX_Layer_",
    typeId = 6,
    Create = function()
        local o = {}
        o.__proto = List_TMX_Layer_
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
BBuffer.Register( List_TMX_Layer_ )
TMX_Layer = {
    typeName = "TMX_Layer",
    typeId = 7,
    Create = function()
        local o = {}
        o.__proto = TMX_Layer
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.id = null -- NullableInt32
        o.name = null -- String
        o.width = 0 -- Int32
        o.height = 0 -- Int32
        o.opacity = 0 -- Double
        o.visible = false -- Boolean
        o.offsetx = 0 -- Double
        o.offsety = 0 -- Double
        o.data = null -- List_TMX_Tile_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        local ReadInt32 = bb.ReadInt32
        local ReadDouble = bb.ReadDouble
        o.id = ReadObject( bb )
        o.name = ReadObject( bb )
        o.width = ReadInt32( bb )
        o.height = ReadInt32( bb )
        o.opacity = ReadDouble( bb )
        o.visible = bb:ReadBoolean()
        o.offsetx = ReadDouble( bb )
        o.offsety = ReadDouble( bb )
        o.data = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        local WriteInt32 = bb.WriteInt32
        local WriteDouble = bb.WriteDouble
        WriteObject( bb, o.id )
        WriteObject( bb, o.name )
        WriteInt32( bb, o.width )
        WriteInt32( bb, o.height )
        WriteDouble( bb, o.opacity )
        bb:WriteBoolean( o.visible )
        WriteDouble( bb, o.offsetx )
        WriteDouble( bb, o.offsety )
        WriteObject( bb, o.data )
    end
}
BBuffer.Register( TMX_Layer )
List_TMX_ObjectGroup_ = {
    typeName = "List_TMX_ObjectGroup_",
    typeId = 8,
    Create = function()
        local o = {}
        o.__proto = List_TMX_ObjectGroup_
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
BBuffer.Register( List_TMX_ObjectGroup_ )
TMX_ObjectGroup = {
    typeName = "TMX_ObjectGroup",
    typeId = 9,
    Create = function()
        local o = {}
        o.__proto = TMX_ObjectGroup
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.id = null -- NullableInt32
        o.name = null -- String
        o.color = null -- TMX_Color4B
        o.opacity = null -- NullableDouble
        o.visible = null -- NullableBoolean
        o.offsetx = null -- NullableDouble
        o.offsety = null -- NullableDouble
        o.draworder = 0 -- TMX_DrawOrderTypes
        o.objects = null -- List_TMX_Object_
        o.properties = null -- List_TMX_Property_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.id = ReadObject( bb )
        o.name = ReadObject( bb )
        o.color = ReadObject( bb )
        o.opacity = ReadObject( bb )
        o.visible = ReadObject( bb )
        o.offsetx = ReadObject( bb )
        o.offsety = ReadObject( bb )
        o.draworder = bb:ReadInt32()
        o.objects = ReadObject( bb )
        o.properties = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.id )
        WriteObject( bb, o.name )
        WriteObject( bb, o.color )
        WriteObject( bb, o.opacity )
        WriteObject( bb, o.visible )
        WriteObject( bb, o.offsetx )
        WriteObject( bb, o.offsety )
        bb:WriteInt32( o.draworder )
        WriteObject( bb, o.objects )
        WriteObject( bb, o.properties )
    end
}
BBuffer.Register( TMX_ObjectGroup )
List_TMX_Property_ = {
    typeName = "List_TMX_Property_",
    typeId = 10,
    Create = function()
        local o = {}
        o.__proto = List_TMX_Property_
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
BBuffer.Register( List_TMX_Property_ )
TMX_Property = {
    typeName = "TMX_Property",
    typeId = 11,
    Create = function()
        local o = {}
        o.__proto = TMX_Property
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.name = null -- String
        o.value = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.name = ReadObject( bb )
        o.value = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.name )
        WriteObject( bb, o.value )
    end
}
BBuffer.Register( TMX_Property )
List_TMX_Tile_ = {
    typeName = "List_TMX_Tile_",
    typeId = 15,
    Create = function()
        local o = {}
        o.__proto = List_TMX_Tile_
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
BBuffer.Register( List_TMX_Tile_ )
TMX_Tile = {
    typeName = "TMX_Tile",
    typeId = 12,
    Create = function()
        local o = {}
        o.__proto = TMX_Tile
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.gid = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.gid = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.gid )
    end
}
BBuffer.Register( TMX_Tile )
List_TMX_Object_ = {
    typeName = "List_TMX_Object_",
    typeId = 13,
    Create = function()
        local o = {}
        o.__proto = List_TMX_Object_
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
BBuffer.Register( List_TMX_Object_ )
TMX_Object = {
    typeName = "TMX_Object",
    typeId = 14,
    Create = function()
        local o = {}
        o.__proto = TMX_Object
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.id = 0 -- Int32
        o.gid = null -- NullableInt32
        o.x = 0 -- Double
        o.y = 0 -- Double
        o.width = 0 -- Double
        o.height = 0 -- Double
        o.name = null -- String
        o.type = null -- String
        o.text = null -- String
        o.visible = false -- Boolean
        o.rotation = 0 -- Double
        o.properties = null -- List_TMX_Property_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        local ReadDouble = bb.ReadDouble
        o.id = bb:ReadInt32()
        o.gid = ReadObject( bb )
        o.x = ReadDouble( bb )
        o.y = ReadDouble( bb )
        o.width = ReadDouble( bb )
        o.height = ReadDouble( bb )
        o.name = ReadObject( bb )
        o.type = ReadObject( bb )
        o.text = ReadObject( bb )
        o.visible = bb:ReadBoolean()
        o.rotation = ReadDouble( bb )
        o.properties = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        local WriteDouble = bb.WriteDouble
        bb:WriteInt32( o.id )
        WriteObject( bb, o.gid )
        WriteDouble( bb, o.x )
        WriteDouble( bb, o.y )
        WriteDouble( bb, o.width )
        WriteDouble( bb, o.height )
        WriteObject( bb, o.name )
        WriteObject( bb, o.type )
        WriteObject( bb, o.text )
        bb:WriteBoolean( o.visible )
        WriteDouble( bb, o.rotation )
        WriteObject( bb, o.properties )
    end
}
BBuffer.Register( TMX_Object )