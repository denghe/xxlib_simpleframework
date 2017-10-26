
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

        local null = _G.null
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
        o.湿最硬 = null -- String
        o.屄拔扶儿 = null -- BBuffer
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb:ReadObject
        o.不淋 = bb:ReadBoolean()
        o.白特 = bb:ReadByte()
        o.撕白特 = bb:ReadSByte()
        o.吸哦特 = bb:ReadInt16()
        o.又吸哦特 = bb:ReadUInt16()
        o.硬特 = bb:ReadInt32()
        o.又硬特 = bb:ReadUInt32()
        o.浪 = bb:ReadInt64()
        o.又浪 = bb:ReadUInt64()
        o.扶裸特 = bb:ReadSingle()
        o.大波 = bb:ReadDouble()
        o.湿最硬 = ReadObject( bb )
        o.屄拔扶儿 = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb:WriteObject
        bb:WriteBoolean( o.不淋 )
        bb:WriteByte( o.白特 )
        bb:WriteSByte( o.撕白特 )
        bb:WriteInt16( o.吸哦特 )
        bb:WriteUInt16( o.又吸哦特 )
        bb:WriteInt32( o.硬特 )
        bb:WriteUInt32( o.又硬特 )
        bb:WriteInt64( o.浪 )
        bb:WriteUInt64( o.又浪 )
        bb:WriteSingle( o.扶裸特 )
        bb:WriteDouble( o.大波 )
        WriteObject( bb, o.湿最硬 )
        WriteObject( bb, o.屄拔扶儿 )
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

        local null = _G.null
        o.立丝特白特 = null -- List_Byte_
        o.立丝特撕白特 = null -- List_SByte_
        o.立丝特吸哦特 = null -- List_Int16_
        o.立丝特又吸哦特 = null -- List_UInt16_
        o.立丝特硬特 = null -- List_Int32_
        o.立丝特又硬特 = null -- List_UInt32_
        o.立丝特浪 = null -- List_Int64_
        o.立丝特又浪 = null -- List_UInt64_
        o.立丝特扶裸特 = null -- List_Single_
        o.立丝特大波 = null -- List_Double_
        o.立丝特湿最硬 = null -- List_String_
        o.立丝特屄拔扶儿 = null -- List_BBuffer_
        --[[
        可以放入 基类 或 派生类
        ]]
        o.立丝特基类 = null -- List_PKG2_基类_
        setmetatable( o, PKG2_基类.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        local ReadObject = bb:ReadObject
        o.立丝特白特 = ReadObject( bb )
        o.立丝特撕白特 = ReadObject( bb )
        o.立丝特吸哦特 = ReadObject( bb )
        o.立丝特又吸哦特 = ReadObject( bb )
        o.立丝特硬特 = ReadObject( bb )
        o.立丝特又硬特 = ReadObject( bb )
        o.立丝特浪 = ReadObject( bb )
        o.立丝特又浪 = ReadObject( bb )
        o.立丝特扶裸特 = ReadObject( bb )
        o.立丝特大波 = ReadObject( bb )
        o.立丝特湿最硬 = ReadObject( bb )
        o.立丝特屄拔扶儿 = ReadObject( bb )
        o.立丝特基类 = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        local WriteObject = bb:WriteObject
        WriteObject( bb, o.立丝特白特 )
        WriteObject( bb, o.立丝特撕白特 )
        WriteObject( bb, o.立丝特吸哦特 )
        WriteObject( bb, o.立丝特又吸哦特 )
        WriteObject( bb, o.立丝特硬特 )
        WriteObject( bb, o.立丝特又硬特 )
        WriteObject( bb, o.立丝特浪 )
        WriteObject( bb, o.立丝特又浪 )
        WriteObject( bb, o.立丝特扶裸特 )
        WriteObject( bb, o.立丝特大波 )
        WriteObject( bb, o.立丝特湿最硬 )
        WriteObject( bb, o.立丝特屄拔扶儿 )
        WriteObject( bb, o.立丝特基类 )
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
        local f = BBuffer.ReadByte
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteByte
        for i = 1, len do
			f( bb, o[ i ] )
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
        local f = BBuffer.ReadSByte
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteSByte
        for i = 1, len do
			f( bb, o[ i ] )
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
        local f = BBuffer.ReadInt16
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteInt16
        for i = 1, len do
			f( bb, o[ i ] )
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
        local f = BBuffer.ReadUInt16
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteUInt16
        for i = 1, len do
			f( bb, o[ i ] )
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
        local f = BBuffer.ReadInt32
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteInt32
        for i = 1, len do
			f( bb, o[ i ] )
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
        local f = BBuffer.ReadUInt32
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteUInt32
        for i = 1, len do
			f( bb, o[ i ] )
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
        local f = BBuffer.ReadInt64
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteInt64
        for i = 1, len do
			f( bb, o[ i ] )
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
        local f = BBuffer.ReadUInt64
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteUInt64
        for i = 1, len do
			f( bb, o[ i ] )
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
        local f = BBuffer.ReadSingle
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteSingle
        for i = 1, len do
			f( bb, o[ i ] )
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
        local f = BBuffer.ReadDouble
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteDouble
        for i = 1, len do
			f( bb, o[ i ] )
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
BBuffer.Register( List_PKG2_基类_ )