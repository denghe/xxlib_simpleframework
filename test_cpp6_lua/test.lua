BBuffer.ReadObject = function( bb )
	local typeId = bb:ReadTypeId()
	if typeId == 0 then
		return BBuffer.null
	end
	local pt = TypeIdProtos[ typeId ]
	if pt == nil then
		error( "invalid typeId: " .. typeId )
	end
	local o, isnew = bb:ReadOffset( pt.Create )
	if isnew ~= nil then
		pt.FromBBuffer( bb, o )
	end
	return o
end
BBuffer.WriteObject = function( bb, o )
	if o == nil or o == BBuffer.null then
		bb:WriteUInt8Zero()
	else
		local pt = o.__prototype
		bb:WriteTypeId( pt.typeId )

		if bb:WriteOffset( o ) == nil then
			pt.ToBBuffer( bb, o )
		end
	end
end
BBuffer.WriteRoot = function( bb, o )
	bb:BeginWrite()
	bb:WriteObject( o )
	bb:EndWrite()
end
BBuffer.ReadRoot = function( bb )
	bb:BeginRead()
	local rtv = bb:ReadObject()
	bb:EndRead()
	return rtv
end

TypeIdProtos = {}	-- 在每个类的位置填充 [ Xxxxxx.typeId ] = Xxxxxx

PKG = PKG or {}		-- namespace PKG
PKG.Node =			-- class Node { Node parent }
{
	typeName = "PKG.Node",
	typeId = 5,
	Create = function()
		local o = {}
		o.__prototype = PKG.Node
		o.__index = o
		o.__newindex = o

		o.node = BBuffer.null	-- PKG.Node
		o.xxx = BBuffer.null	-- string
		-- setmetatable(o, BaseProto)
		return o
	end,
	FromBBuffer = function( bb, o )
		rawset( o, "node", bb:ReadObject() )
		rawset( o, "xxx", bb:ReadString() )
	end,
	ToBBuffer = function( bb, o )
		bb:WriteObject( rawget( o, "node" ) )
		bb:WriteString( rawget( o, "xxx" ) )
	end
}
TypeIdProtos[ PKG.Node.typeId ] = PKG.Node

-- 测试
local node = PKG.Node.Create()
node.node = node
node.xxx = "1234567"

local bb = BBuffer.Create()
bb:WriteRoot( node )
print( bb:Dump() )
local node2 = bb:ReadRoot()
print( node2.xxx )


--[[
local bb = BBuffer.Create()
local xxx = PKG.Xxx.Create()
xxx.serial = 1
xxx.ticks = 22
xxx.msg = "333"

print( rawget( xxx, "serial" ), xxx.serial )
print( rawget( xxx, "ticks" ), xxx.ticks )
print( rawget( xxx, "msg" ), xxx.msg )

print( "!!!!!!!!!!!!!!!!!!!" )

bb:WriteObject( xxx )
print( bb:Dump() )

local eee = bb:ReadObject()
print( rawget( eee, "serial" ), eee.serial )
print( rawget( eee, "ticks" ), eee.ticks )
print( rawget( eee, "msg" ), eee.msg )
]]


















--[[



--[[
-- class Request { int serial }
PKG.Request = 
{
	typeName = "PKG.Request",
	typeId = 3,
	Create = function()
		local o = {}
		o.__prototype = PKG.Request
		o.__index = o
		o.__newindex = o

		o.serial = 0
		-- setmetatable(o, BaseProto)
		return o
	end,
	FromBBuffer = function( bb, o )
		rawset( o, "serial", bb:ReadInt32() )
	end,
	ToBBuffer = function( bb, o )
		bb:WriteInt32( rawget( o, "serial" ) )
	end
}
TypeIdProtos[ PKG.Request.typeId ] = PKG.Request

-- class Xxx : Request { long ticks, string msg }
PKG.Xxx = 
{
	typeName = "PKG.Xxx",
	typeId = 4,
	Create = function()
		local o = {}
		o.__prototype = PKG.Xxx
		o.__index = o
		o.__newindex = o

		o.ticks = 0
		o.msg = BBuffer.null	-- 占位. 如果不占位, 赋值的时候将被递归到 metatables 中
		setmetatable( o, PKG.Request.Create() )
		return o
	end,
	FromBBuffer = function( bb, o )
		local p = getmetatable( o )
		p.__prototype.FromBBuffer( bb, p )
		rawset( o, "ticks" ,bb:ReadInt64() )
		rawset( o, "msg" ,bb:ReadString() )
	end,
	ToBBuffer = function( bb, o )
		local p = getmetatable( o )
		p.__prototype.ToBBuffer( bb, p )
		bb:WriteInt64( rawget( o, "ticks" ) )
		bb:WriteString( rawget( o, "msg" ) )
	end
}
TypeIdProtos[ PKG.Xxx.typeId ] = PKG.Xxx




local bb = BBuffer.Create()
local bb2 = BBuffer.Create()
bb2:WriteUInt8( 255 )
bb2:WriteString( "中文" )
bb:WriteBBuffer( bb2 )
print( bb:Dump() )
local bb3 = bb:ReadBBuffer()
print( bb:Dump() )
print( bb3:Dump() )

ParentTypeIds = {}
ParentTypeIds[ 1 ] = 0			-- String : Object
ParentTypeIds[ 2 ] = 0			-- BBuffer : Object
ParentTypeIds[ 3 ] = 0			-- PKG.Request : Object
ParentTypeIds[ 4 ] = 3			-- PKG.Xxx : PKG.Request
ParentTypeIds[ 5 ] = 0			-- PKG.Node : Object
-- more


-- 传入父子 id 号, 检测其关系是否成立( 不判断 nil )
function IsBaseOf( p, t )
	::begin::
	if p == t then return true end
	if t == 0 then return false end
	t = ParentTypeIds[ t ]
	goto begin
end

-- 传入当前原型 与 读取到的 typeId, 判断父子关系, 如果原型 id == typeId, 则直接返回原型
function TypeIdBaseOfCheck( pt, typeId )
	if ~IsBaseOf( pt.typeId, typeId ) then
		error( "IsBaseOf check fail. "..pt.typeName..".typeId is "..pt.typeId..", read typeId is "..typeId )
	end
	if pt.typeId == typeId then
		return pt
	else
		return TypeIdProtos[ typeId ]
	end
end

]]
