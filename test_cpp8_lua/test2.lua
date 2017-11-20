print( "TestLua" )

dofile( "pkg2_class.lua" )
local bb = BBuffer.Create()
local o = PKG2_基类.Create()
o.不淋 = true -- Boolean
o.白特 = 1 -- Byte
o.撕白特 = -2 -- SByte
o.吸哦特 = -3 -- Int16
o.又吸哦特 = 4 -- UInt16
o.硬特 = -5 -- Int32
o.又硬特 = 6 -- UInt32
o.浪 = -7 -- Int64
o.又浪 = 8 -- UInt64
o.扶裸特 = 9.1 -- Single
o.大波 = 10.2 -- Double
o.湿最硬 = "这是个串儿" -- String
o.屄拔扶儿 = BBuffer.Create() -- BBuffer
o.屄拔扶儿:WriteByte( 1 )
o.屄拔扶儿:WriteByte( 2 )
o.屄拔扶儿:WriteByte( 3 )

local p = PKG2_派生类.Create()
p.立丝特基类 = List_PKG2_基类_.Create()
table.insert( p.立丝特基类, p )
table.insert( p.立丝特基类, o )
p.立丝特屄拔扶儿 = List_BBuffer_.Create()
table.insert( p.立丝特屄拔扶儿, BBuffer.Create() )
p.立丝特白特 = List_Byte_.Create()
table.insert( p.立丝特白特, 1 )
table.insert( p.立丝特白特, 2 )
table.insert( p.立丝特白特, 3 )
bb:Clear()
bb:WriteRoot( p )
print( bb )


local p2 = bb:ReadRoot()
