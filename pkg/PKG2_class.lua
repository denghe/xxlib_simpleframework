
--[[
测试一下各种数据类型
]]
PKG2_基类 = {
    typeName = "PKG2_基类",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = PKG2_基类
        o.__index = o
        o.__newindex = o

        o.不淋 = false -- Boolean
        o.白特 = 0 -- Byte
        o.撕白特 = 0 -- SByte
        o.吸哦特 = 0 -- Int16
        o.又吸哦特 = 0 -- UInt16
        o.硬特 = 0 -- Int32
        o.又硬特 = 0 -- UInt32
        o.浪 = 0 -- Int64
        o.又浪 = 0 -- UInt64
        o.扶裸特 = 0 -- Single
        o.大波 = 0 -- Double
        o.湿最硬 = BBuffer.null -- String
        o.屄拔扶儿 = BBuffer.null -- BBuffer
        return o
    end,
    FromBBuffer = function( bb, o )
        rawset( o, "不淋", bb:ReadBoolean() )
        rawset( o, "白特", bb:ReadByte() )
        rawset( o, "撕白特", bb:ReadSByte() )
        rawset( o, "吸哦特", bb:ReadInt16() )
        rawset( o, "又吸哦特", bb:ReadUInt16() )
        rawset( o, "硬特", bb:ReadInt32() )
        rawset( o, "又硬特", bb:ReadUInt32() )
        rawset( o, "浪", bb:ReadInt64() )
        rawset( o, "又浪", bb:ReadUInt64() )
        rawset( o, "扶裸特", bb:ReadSingle() )
        rawset( o, "大波", bb:ReadDouble() )
        rawset( o, "湿最硬", bb:ReadObject() )
        rawset( o, "屄拔扶儿", bb:ReadObject() )
    end,
    ToBBuffer = function( bb, o )
        bb:WriteBoolean( rawget( o, "不淋" ) )
        bb:WriteByte( rawget( o, "白特" ) )
        bb:WriteSByte( rawget( o, "撕白特" ) )
        bb:WriteInt16( rawget( o, "吸哦特" ) )
        bb:WriteUInt16( rawget( o, "又吸哦特" ) )
        bb:WriteInt32( rawget( o, "硬特" ) )
        bb:WriteUInt32( rawget( o, "又硬特" ) )
        bb:WriteInt64( rawget( o, "浪" ) )
        bb:WriteUInt64( rawget( o, "又浪" ) )
        bb:WriteSingle( rawget( o, "扶裸特" ) )
        bb:WriteDouble( rawget( o, "大波" ) )
        bb:WriteObject( rawget( o, "湿最硬" ) )
        bb:WriteObject( rawget( o, "屄拔扶儿" ) )
    end
}
BBuffer.Register( PKG2_基类 )
PKG2_派生类 = {
    typeName = "PKG2_派生类",
    typeId = 4,
    Create = function()
        local o = {}
        o.__proto = PKG2_派生类
        o.__index = o
        o.__newindex = o

        o.立丝特白特 = BBuffer.null -- List_Byte_
        o.立丝特撕白特 = BBuffer.null -- List_SByte_
        o.立丝特吸哦特 = BBuffer.null -- List_Int16_
        o.立丝特又吸哦特 = BBuffer.null -- List_UInt16_
        o.立丝特硬特 = BBuffer.null -- List_Int32_
        o.立丝特又硬特 = BBuffer.null -- List_UInt32_
        o.立丝特浪 = BBuffer.null -- List_Int64_
        o.立丝特又浪 = BBuffer.null -- List_UInt64_
        o.立丝特扶裸特 = BBuffer.null -- List_Single_
        o.立丝特大波 = BBuffer.null -- List_Double_
        o.立丝特湿最硬 = BBuffer.null -- List_String_
        o.立丝特屄拔扶儿 = BBuffer.null -- List_BBuffer_
        --[[
        可以放入 基类 或 派生类
        ]]
        o.立丝特基类 = BBuffer.null -- List_PKG2_基类_
        setmetatable( o, PKG2_基类.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        rawget( p, "__proto" ).FromBBuffer( bb, p )
        rawset( o, "立丝特白特", bb:ReadObject() )
        rawset( o, "立丝特撕白特", bb:ReadObject() )
        rawset( o, "立丝特吸哦特", bb:ReadObject() )
        rawset( o, "立丝特又吸哦特", bb:ReadObject() )
        rawset( o, "立丝特硬特", bb:ReadObject() )
        rawset( o, "立丝特又硬特", bb:ReadObject() )
        rawset( o, "立丝特浪", bb:ReadObject() )
        rawset( o, "立丝特又浪", bb:ReadObject() )
        rawset( o, "立丝特扶裸特", bb:ReadObject() )
        rawset( o, "立丝特大波", bb:ReadObject() )
        rawset( o, "立丝特湿最硬", bb:ReadObject() )
        rawset( o, "立丝特屄拔扶儿", bb:ReadObject() )
        rawset( o, "立丝特基类", bb:ReadObject() )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        rawget( p, "__proto" ).ToBBuffer( bb, p )
        bb:WriteObject( rawget( o, "立丝特白特" ) )
        bb:WriteObject( rawget( o, "立丝特撕白特" ) )
        bb:WriteObject( rawget( o, "立丝特吸哦特" ) )
        bb:WriteObject( rawget( o, "立丝特又吸哦特" ) )
        bb:WriteObject( rawget( o, "立丝特硬特" ) )
        bb:WriteObject( rawget( o, "立丝特又硬特" ) )
        bb:WriteObject( rawget( o, "立丝特浪" ) )
        bb:WriteObject( rawget( o, "立丝特又浪" ) )
        bb:WriteObject( rawget( o, "立丝特扶裸特" ) )
        bb:WriteObject( rawget( o, "立丝特大波" ) )
        bb:WriteObject( rawget( o, "立丝特湿最硬" ) )
        bb:WriteObject( rawget( o, "立丝特屄拔扶儿" ) )
        bb:WriteObject( rawget( o, "立丝特基类" ) )
    end
}
BBuffer.Register( PKG2_派生类 )
List_Byte_ = {
    typeName = "List_Byte_",
    typeId = 5,
    Create = function()
        local o = {}
        o.__proto = List_Byte_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadByte() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteByte( v )
		end
    end
}
BBuffer.Register( List_Byte_ )
List_SByte_ = {
    typeName = "List_SByte_",
    typeId = 6,
    Create = function()
        local o = {}
        o.__proto = List_SByte_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadSByte() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteSByte( v )
		end
    end
}
BBuffer.Register( List_SByte_ )
List_Int16_ = {
    typeName = "List_Int16_",
    typeId = 7,
    Create = function()
        local o = {}
        o.__proto = List_Int16_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadInt16() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteInt16( v )
		end
    end
}
BBuffer.Register( List_Int16_ )
List_UInt16_ = {
    typeName = "List_UInt16_",
    typeId = 8,
    Create = function()
        local o = {}
        o.__proto = List_UInt16_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadUInt16() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteUInt16( v )
		end
    end
}
BBuffer.Register( List_UInt16_ )
List_Int32_ = {
    typeName = "List_Int32_",
    typeId = 9,
    Create = function()
        local o = {}
        o.__proto = List_Int32_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadInt32() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteInt32( v )
		end
    end
}
BBuffer.Register( List_Int32_ )
List_UInt32_ = {
    typeName = "List_UInt32_",
    typeId = 10,
    Create = function()
        local o = {}
        o.__proto = List_UInt32_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadUInt32() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteUInt32( v )
		end
    end
}
BBuffer.Register( List_UInt32_ )
List_Int64_ = {
    typeName = "List_Int64_",
    typeId = 11,
    Create = function()
        local o = {}
        o.__proto = List_Int64_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadInt64() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteInt64( v )
		end
    end
}
BBuffer.Register( List_Int64_ )
List_UInt64_ = {
    typeName = "List_UInt64_",
    typeId = 12,
    Create = function()
        local o = {}
        o.__proto = List_UInt64_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadUInt64() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteUInt64( v )
		end
    end
}
BBuffer.Register( List_UInt64_ )
List_Single_ = {
    typeName = "List_Single_",
    typeId = 13,
    Create = function()
        local o = {}
        o.__proto = List_Single_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadSingle() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteSingle( v )
		end
    end
}
BBuffer.Register( List_Single_ )
List_Double_ = {
    typeName = "List_Double_",
    typeId = 14,
    Create = function()
        local o = {}
        o.__proto = List_Double_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadDouble() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteDouble( v )
		end
    end
}
BBuffer.Register( List_Double_ )
List_String_ = {
    typeName = "List_String_",
    typeId = 15,
    Create = function()
        local o = {}
        o.__proto = List_String_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadObject() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteObject( v )
		end
    end
}
BBuffer.Register( List_String_ )
List_BBuffer_ = {
    typeName = "List_BBuffer_",
    typeId = 16,
    Create = function()
        local o = {}
        o.__proto = List_BBuffer_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadObject() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteObject( v )
		end
    end
}
BBuffer.Register( List_BBuffer_ )
List_PKG2_基类_ = {
    typeName = "List_PKG2_基类_",
    typeId = 17,
    Create = function()
        local o = {}
        o.__proto = List_PKG2_基类_
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:ReadObject() )
		end
    end,
    ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteObject( v )
		end
    end
}
BBuffer.Register( List_PKG2_基类_ )