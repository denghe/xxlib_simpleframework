-- todo: 下面几个函数的实现, 做进 cpp, TypeIdProtos 移进 BBuffer, 为 BBuffer 提供 Register 函数以映射 typeId -- Create 函数
-- todo: List, enum support. List 根据 __childprototype 的指向来路由子的处理函数

--[[
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
		local pt = o.__proto
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
]]



-- 扩展一下 table 的功能以便于使用
if table.__index ~= nil then
	error( "table.__index ~= nil" )
end

-- 准备拿 table 来当元表
table.__index = table

-- 顺应一下使用习惯
table.Add = table.insert
table.Insert = table.insert
table.Remove = table.remove

-- 实现一个 Dump 功能以方便查看结果
table.Dump = function()
	for k, v in ipairs( self ) do
		print( k, v )
	end
end




TypeIdProtos = {}	-- 在每个类的位置填充 [ Xxxxxx.typeId ] = Xxxxxx

PKG_Node =			-- class PKG.Node { PKG.Node parent; List<PKG.Node> childs; string msg; }
{
	typeName = "PKG_Node",
	typeId = 3,
	Create = function()
		local o = {}
		o.__proto = PKG_Node
		o.__index = o
		o.__newindex = o

		o.parent = BBuffer.null	-- PKG.Node
		o.childs = BBuffer.null	-- List<PKG.Node>
		o.ids = BBuffer.null	-- List_Int64_
		o.msg = BBuffer.null	-- string

		-- setmetatable( o, BaseProto.Create() )
		return o
	end,
	FromBBuffer = function( bb, o )
		rawset( o, "parent", bb:ReadObject() )
		rawset( o, "childs", bb:ReadObject() )
		rawset( o, "ids", bb:ReadObject() )
		rawset( o, "msg", bb:ReadString() )
	end,
	ToBBuffer = function( bb, o )
		bb:WriteObject( rawget( o, "parent" ) )
		bb:WriteObject( rawget( o, "childs" ) )
		bb:WriteObject( rawget( o, "ids" ) )
		bb:WriteString( rawget( o, "msg" ) )
	end
}
TypeIdProtos[ PKG_Node.typeId ] = PKG_Node

List_PKG_Node_ =		-- List<PKG.Node>
{
	typeName = "List_PKG_Node_",
	typeId = 4,
	Create = function()
		local o = {}
		o.__proto = List_PKG_Node_
		o.__index = o
		o.__newindex = o

		setmetatable( o, table )
		return o
	end,
	FromBBuffer = function( bb, o )
		--bb:ReadList( o )
	end,
	ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteObject( v )
		end
	end
}
TypeIdProtos[ List_PKG_Node_.typeId ] = List_PKG_Node_

List_Int64_ =			-- List<long>
{
	typeName = "List_Int64_",
	typeId = -5,
	Create = function()
		local o = {}
		o.__proto = List_Int64_
		o.__childprototype = BBuffer.Int64
		o.__index = o
		o.__newindex = o

		setmetatable( o, table )
		return o
	end,
	FromBBuffer = function( bb, o )
		--bb:ReadList( o )
	end,
	ToBBuffer = function( bb, o )
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:WriteInt64( v )
		end
	end
}
TypeIdProtos[ List_PKG_Node_.typeId ] = List_PKG_Node_

-- List_Boolean 编码方式比较特殊, 暂时先不支持

-- 测试

local node = PKG_Node.Create()

node.parent = node

node.childs = List_PKG_Node_.Create()
node.childs:Add( node )
node.childs:Add( node )

node.ids = List_Int64_.Create()
node.ids:Add( 1 )
node.ids:Add( 2 )
node.ids:Add( 3 )

node.msg = "1234567"

local bb = BBuffer.Create()
bb:WriteRoot( node )
print( bb:Dump() )

--local node2 = bb:ReadRoot()
--print( node2.msg )























    
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
		o.__proto = PKG.Request
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
		o.__proto = PKG.Xxx
		o.__index = o
		o.__newindex = o

		o.ticks = 0
		o.msg = BBuffer.null	-- 占位. 如果不占位, 赋值的时候将被递归到 metatables 中
		setmetatable( o, PKG.Request.Create() )
		return o
	end,
	FromBBuffer = function( bb, o )
		local p = getmetatable( o )
		p.__proto.FromBBuffer( bb, p )
		rawset( o, "ticks" ,bb:ReadInt64() )
		rawset( o, "msg" ,bb:ReadString() )
	end,
	ToBBuffer = function( bb, o )
		local p = getmetatable( o )
		p.__proto.ToBBuffer( bb, p )
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
