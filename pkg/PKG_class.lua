
PKG_PkgGenMd5_Value = '573f3204de5ce9a0f9f76111d81176c6'

--[[
进入游戏成功
]]
PKG_CatchFish_Client_JoinSuccess = {
    typeName = "PKG_CatchFish_Client_JoinSuccess",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Client_JoinSuccess
        o.__index = o
        o.__newindex = o

        --[[
        座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        ]]
        o.sitIndex = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.sitIndex = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.sitIndex )
    end
}
BBuffer.Register( PKG_CatchFish_Client_JoinSuccess )
--[[
进入失败
]]
PKG_CatchFish_Client_JoinFail = {
    typeName = "PKG_CatchFish_Client_JoinFail",
    typeId = 4,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Client_JoinFail
        o.__index = o
        o.__newindex = o

        --[[
        错误提示信息
        ]]
        o.errMsg = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.errMsg = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.errMsg )
    end
}
BBuffer.Register( PKG_CatchFish_Client_JoinFail )
--[[
要按成员顺序优先级依次处理
]]
PKG_CatchFish_Client_FullSync = {
    typeName = "PKG_CatchFish_Client_FullSync",
    typeId = 5,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Client_FullSync
        o.__index = o
        o.__newindex = o

        o.scene = null -- PKG_CatchFish_Scene
        return o
    end,
    FromBBuffer = function( bb, o )
        o.scene = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.scene )
    end
}
BBuffer.Register( PKG_CatchFish_Client_FullSync )
--[[
场景
]]
PKG_CatchFish_Scene = {
    typeName = "PKG_CatchFish_Scene",
    typeId = 6,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Scene
        o.__index = o
        o.__newindex = o

        --[[
        帧编号, 每帧 + 1
        ]]
        o.frameNumber = 0 -- Int32
        --[[
        公用随机数发生器
        ]]
        o.rnd = null -- _xx_Random
        --[[
        公用配置信息
        ]]
        o.cfg = null -- PKG_CatchFish_Config
        --[[
        所有玩家( 子弹在玩家下面 )
        ]]
        o.players = null -- List_PKG_CatchFish_Player_
        --[[
        所有鱼
        ]]
        o.fishs = null -- List_PKG_CatchFish_Fish_
        --[[
        当前帧所有事件的合并包容器, 服务器专用, 不参与网络传输
        ]]
        o.frameEvents = null -- PKG_CatchFish_Client_FrameEvents
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.frameNumber = bb:ReadInt32()
        o.rnd = ReadObject( bb )
        o.cfg = ReadObject( bb )
        o.players = ReadObject( bb )
        o.fishs = ReadObject( bb )
        o.frameEvents = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        bb:WriteInt32( o.frameNumber )
        WriteObject( bb, o.rnd )
        WriteObject( bb, o.cfg )
        WriteObject( bb, o.players )
        WriteObject( bb, o.fishs )
        WriteObject( bb, o.frameEvents )
    end
}
BBuffer.Register( PKG_CatchFish_Scene )
--[[
要按成员顺序优先级依次处理
]]
PKG_CatchFish_Client_FrameEvents = {
    typeName = "PKG_CatchFish_Client_FrameEvents",
    typeId = 7,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Client_FrameEvents
        o.__index = o
        o.__newindex = o

        --[[
        当时的帧编号
        ]]
        o.frameNumber = 0 -- Int32
        --[[
        一帧内所有离开的玩家列表( 有序 )
        ]]
        o.leaves = null -- List_PKG_CatchFish_Events_LeavePlayer_
        --[[
        一帧内所有进入的玩家列表( 有序 )
        ]]
        o.joins = null -- List_PKG_CatchFish_Events_JoinPlayer_
        --[[
        多个玩家的子弹 命中 信息( 相同玩家可能有多条 )
        ]]
        o.hitss = null -- List_PKG_CatchFish_Events_BulletHit_
        --[[
        多条鱼 死亡 & 结算 信息
        ]]
        o.fishDeads = null -- List_PKG_CatchFish_Events_FishDead_
        --[[
        多个玩家的单次开火信息( 自带 frameNumber, 可能小于当前值 )
        ]]
        o.fires = null -- List_PKG_CatchFish_Events_Fire_
        --[[
        多个玩家的持续开火结束信息
        ]]
        o.fireEnds = null -- List_PKG_CatchFish_Events_FireEnd_
        --[[
        多个玩家的持续开火开始信息
        ]]
        o.fireBegins = null -- List_PKG_CatchFish_Events_FireBegin_
        --[[
        多个玩家的持续开火角度调整信息
        ]]
        o.fireChangeAngles = null -- List_PKG_CatchFish_Events_FireChangeAngle_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.frameNumber = bb:ReadInt32()
        o.leaves = ReadObject( bb )
        o.joins = ReadObject( bb )
        o.hitss = ReadObject( bb )
        o.fishDeads = ReadObject( bb )
        o.fires = ReadObject( bb )
        o.fireEnds = ReadObject( bb )
        o.fireBegins = ReadObject( bb )
        o.fireChangeAngles = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        bb:WriteInt32( o.frameNumber )
        WriteObject( bb, o.leaves )
        WriteObject( bb, o.joins )
        WriteObject( bb, o.hitss )
        WriteObject( bb, o.fishDeads )
        WriteObject( bb, o.fires )
        WriteObject( bb, o.fireEnds )
        WriteObject( bb, o.fireBegins )
        WriteObject( bb, o.fireChangeAngles )
    end
}
BBuffer.Register( PKG_CatchFish_Client_FrameEvents )
List_PKG_CatchFish_Events_LeavePlayer_ = {
    typeName = "List_PKG_CatchFish_Events_LeavePlayer_",
    typeId = 8,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_Events_LeavePlayer_
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
BBuffer.Register( List_PKG_CatchFish_Events_LeavePlayer_ )
List_PKG_CatchFish_Events_JoinPlayer_ = {
    typeName = "List_PKG_CatchFish_Events_JoinPlayer_",
    typeId = 9,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_Events_JoinPlayer_
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
BBuffer.Register( List_PKG_CatchFish_Events_JoinPlayer_ )
List_PKG_CatchFish_Events_BulletHit_ = {
    typeName = "List_PKG_CatchFish_Events_BulletHit_",
    typeId = 10,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_Events_BulletHit_
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
BBuffer.Register( List_PKG_CatchFish_Events_BulletHit_ )
List_PKG_CatchFish_Events_FishDead_ = {
    typeName = "List_PKG_CatchFish_Events_FishDead_",
    typeId = 11,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_Events_FishDead_
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
BBuffer.Register( List_PKG_CatchFish_Events_FishDead_ )
List_PKG_CatchFish_Events_Fire_ = {
    typeName = "List_PKG_CatchFish_Events_Fire_",
    typeId = 12,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_Events_Fire_
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
BBuffer.Register( List_PKG_CatchFish_Events_Fire_ )
List_PKG_CatchFish_Events_FireEnd_ = {
    typeName = "List_PKG_CatchFish_Events_FireEnd_",
    typeId = 13,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_Events_FireEnd_
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
BBuffer.Register( List_PKG_CatchFish_Events_FireEnd_ )
List_PKG_CatchFish_Events_FireBegin_ = {
    typeName = "List_PKG_CatchFish_Events_FireBegin_",
    typeId = 14,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_Events_FireBegin_
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
BBuffer.Register( List_PKG_CatchFish_Events_FireBegin_ )
List_PKG_CatchFish_Events_FireChangeAngle_ = {
    typeName = "List_PKG_CatchFish_Events_FireChangeAngle_",
    typeId = 15,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_Events_FireChangeAngle_
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
BBuffer.Register( List_PKG_CatchFish_Events_FireChangeAngle_ )
--[[
客户端与服务器建立连接后的首包
]]
PKG_Client_CatchFish_Join = {
    typeName = "PKG_Client_CatchFish_Join",
    typeId = 16,
    Create = function()
        local o = {}
        o.__proto = PKG_Client_CatchFish_Join
        o.__index = o
        o.__newindex = o

        --[[
        客户端进游戏前自己填的名字. 可以写磁盘存在本地.
        ]]
        o.username = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.username = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.username )
    end
}
BBuffer.Register( PKG_Client_CatchFish_Join )
--[[
正常离开游戏( 立即, 并非断线等待超时后清掉 )
]]
PKG_Client_CatchFish_Leave = {
    typeName = "PKG_Client_CatchFish_Leave",
    typeId = 17,
    Create = function()
        local o = {}
        o.__proto = PKG_Client_CatchFish_Leave
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
    end,
    ToBBuffer = function( bb, o )
    end
}
BBuffer.Register( PKG_Client_CatchFish_Leave )
--[[
开火( 单次, 或频繁单次模拟伪连发 )
]]
PKG_Client_CatchFish_Fire = {
    typeName = "PKG_Client_CatchFish_Fire",
    typeId = 18,
    Create = function()
        local o = {}
        o.__proto = PKG_Client_CatchFish_Fire
        o.__index = o
        o.__newindex = o

        --[[
        当时的帧编号
        ]]
        o.frameNumber = 0 -- Int32
        --[[
        子弹流水号
        ]]
        o.bulletSerial = 0 -- Int32
        --[[
        金币价值( 也可理解为倍率 )
        ]]
        o.coin = 0 -- Int64
        --[[
        射击角度( 相对于炮台自己的正方向角度 )
        ]]
        o.angle = 0 -- Single
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.frameNumber = ReadInt32( bb )
        o.bulletSerial = ReadInt32( bb )
        o.coin = bb:ReadInt64()
        o.angle = bb:ReadSingle()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.frameNumber )
        WriteInt32( bb, o.bulletSerial )
        bb:WriteInt64( o.coin )
        bb:WriteSingle( o.angle )
    end
}
BBuffer.Register( PKG_Client_CatchFish_Fire )
--[[
玩家开始开火( 连发, 仅适合帧同步服务器算法 )
]]
PKG_Client_CatchFish_FireBegin = {
    typeName = "PKG_Client_CatchFish_FireBegin",
    typeId = 19,
    Create = function()
        local o = {}
        o.__proto = PKG_Client_CatchFish_FireBegin
        o.__index = o
        o.__newindex = o

        --[[
        射击角度( 相对于炮台自己的正方向角度 )
        ]]
        o.angle = 0 -- Single
        return o
    end,
    FromBBuffer = function( bb, o )
        o.angle = bb:ReadSingle()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteSingle( o.angle )
    end
}
BBuffer.Register( PKG_Client_CatchFish_FireBegin )
--[[
玩家持续开火时调整角度( 连发, 仅适合帧同步服务器算法 )
]]
PKG_Client_CatchFish_FireChangeAngle = {
    typeName = "PKG_Client_CatchFish_FireChangeAngle",
    typeId = 20,
    Create = function()
        local o = {}
        o.__proto = PKG_Client_CatchFish_FireChangeAngle
        o.__index = o
        o.__newindex = o

        --[[
        射击角度( 相对于炮台自己的正方向角度 )
        ]]
        o.angle = 0 -- Single
        return o
    end,
    FromBBuffer = function( bb, o )
        o.angle = bb:ReadSingle()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteSingle( o.angle )
    end
}
BBuffer.Register( PKG_Client_CatchFish_FireChangeAngle )
--[[
玩家停止持续开火( 连发, 仅适合帧同步服务器算法 )
]]
PKG_Client_CatchFish_FireEnd = {
    typeName = "PKG_Client_CatchFish_FireEnd",
    typeId = 21,
    Create = function()
        local o = {}
        o.__proto = PKG_Client_CatchFish_FireEnd
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
    end,
    ToBBuffer = function( bb, o )
    end
}
BBuffer.Register( PKG_Client_CatchFish_FireEnd )
--[[
圆形碰撞区
]]
PKG_CatchFish_CollisionArea = {
    typeName = "PKG_CatchFish_CollisionArea",
    typeId = 22,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_CollisionArea
        o.__index = o
        o.__newindex = o

        --[[
        初始方向的位置偏移( 运算时要根据角度来算实际偏移值 )
        ]]
        o.offset = null -- _xx_Pos
        --[[
        初始方向的位置偏移( 按 256 划分精度的角度值为下标, 运行时先行计算出来 )
        ]]
        o.offsets = null -- List__xx_Pos_
        --[[
        半径
        ]]
        o.radius = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.offset = ReadObject( bb )
        o.offsets = ReadObject( bb )
        o.radius = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.offset )
        WriteObject( bb, o.offsets )
        bb:WriteInt32( o.radius )
    end
}
BBuffer.Register( PKG_CatchFish_CollisionArea )
List__xx_Pos_ = {
    typeName = "List__xx_Pos_",
    typeId = 23,
    Create = function()
        local o = {}
        o.__proto = List__xx_Pos_
        o.__index = o
        o.__newindex = o
        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
        local f = BBuffer.ReadPos
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WritePos
        for i = 1, len do
			f( bb, o[ i ] )
		end
    end
}
BBuffer.Register( List__xx_Pos_ )
--[[
鱼帧动画信息
]]
PKG_CatchFish_FishSpriteFrame = {
    typeName = "PKG_CatchFish_FishSpriteFrame",
    typeId = 24,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_FishSpriteFrame
        o.__index = o
        o.__newindex = o

        --[[
        打包前的散文件名
        ]]
        o.fileName = null -- String
        --[[
        基于当前帧图的圆形碰撞区明细表
        ]]
        o.collisionAreas = null -- List_PKG_CatchFish_CollisionArea_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.fileName = ReadObject( bb )
        o.collisionAreas = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.fileName )
        WriteObject( bb, o.collisionAreas )
    end
}
BBuffer.Register( PKG_CatchFish_FishSpriteFrame )
List_PKG_CatchFish_CollisionArea_ = {
    typeName = "List_PKG_CatchFish_CollisionArea_",
    typeId = 25,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_CollisionArea_
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
BBuffer.Register( List_PKG_CatchFish_CollisionArea_ )
--[[
鱼配置信息
]]
PKG_CatchFish_FishConfig = {
    typeName = "PKG_CatchFish_FishConfig",
    typeId = 26,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_FishConfig
        o.__index = o
        o.__newindex = o

        --[[
        鱼类编号
        ]]
        o.typeId = 0 -- Int32
        --[[
        鱼名
        ]]
        o.name = null -- String
        --[[
        打死鱼的金币所得基数( 也可理解成倍率 )
        ]]
        o.coin = 0 -- Int64
        --[[
        基于整个鱼的最大晃动范围的圆形检测区( 粗判 )
        ]]
        o.collisionArea = null -- PKG_CatchFish_CollisionArea
        --[[
        鱼帧动画信息集合
        ]]
        o.frames = null -- List_PKG_CatchFish_FishSpriteFrame_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.typeId = bb:ReadInt32()
        o.name = ReadObject( bb )
        o.coin = bb:ReadInt64()
        o.collisionArea = ReadObject( bb )
        o.frames = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        bb:WriteInt32( o.typeId )
        WriteObject( bb, o.name )
        bb:WriteInt64( o.coin )
        WriteObject( bb, o.collisionArea )
        WriteObject( bb, o.frames )
    end
}
BBuffer.Register( PKG_CatchFish_FishConfig )
List_PKG_CatchFish_FishSpriteFrame_ = {
    typeName = "List_PKG_CatchFish_FishSpriteFrame_",
    typeId = 27,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_FishSpriteFrame_
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
BBuffer.Register( List_PKG_CatchFish_FishSpriteFrame_ )
--[[
游戏配置信息( 配置信息并不会随着网络同步而下发, 反序列化后需要手工还原 )
]]
PKG_CatchFish_Config = {
    typeName = "PKG_CatchFish_Config",
    typeId = 28,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Config
        o.__index = o
        o.__newindex = o

        --[[
        所有鱼的配置信息
        ]]
        o.fishCfgs = null -- List_PKG_CatchFish_FishConfig_
        --[[
        子弹配置( 当前就一种子弹，先这样放置 )
        ]]
        o.bulletRadius = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.fishCfgs = bb:ReadObject()
        o.bulletRadius = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.fishCfgs )
        bb:WriteInt32( o.bulletRadius )
    end
}
BBuffer.Register( PKG_CatchFish_Config )
List_PKG_CatchFish_FishConfig_ = {
    typeName = "List_PKG_CatchFish_FishConfig_",
    typeId = 29,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_FishConfig_
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
BBuffer.Register( List_PKG_CatchFish_FishConfig_ )
--[[
玩家
]]
PKG_CatchFish_Player = {
    typeName = "PKG_CatchFish_Player",
    typeId = 30,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Player
        o.__index = o
        o.__newindex = o

        --[[
        位于容器时的下标
        ]]
        o.indexAtContainer = 0 -- Int32
        --[[
        玩家名字
        ]]
        o.name = null -- String
        --[[
        座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 ), 同时也用于标识玩家
        ]]
        o.sitIndex = 0 -- Int32
        --[[
        当前金币数
        ]]
        o.coin = 0 -- Int64
        --[[
        所有子弹
        ]]
        o.bullets = null -- List_PKG_CatchFish_Bullet_
        --[[
        玩家网络上下文, 不参与网络传输
        ]]
        o.ctx = null -- _PlayerContext
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadObject = bb.ReadObject
        o.indexAtContainer = ReadInt32( bb )
        o.name = ReadObject( bb )
        o.sitIndex = ReadInt32( bb )
        o.coin = bb:ReadInt64()
        o.bullets = ReadObject( bb )
        o.ctx = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteObject = bb.WriteObject
        WriteInt32( bb, o.indexAtContainer )
        WriteObject( bb, o.name )
        WriteInt32( bb, o.sitIndex )
        bb:WriteInt64( o.coin )
        WriteObject( bb, o.bullets )
        WriteObject( bb, o.ctx )
    end
}
BBuffer.Register( PKG_CatchFish_Player )
List_PKG_CatchFish_Bullet_ = {
    typeName = "List_PKG_CatchFish_Bullet_",
    typeId = 31,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_Bullet_
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
BBuffer.Register( List_PKG_CatchFish_Bullet_ )
_PlayerContext = {
    typeName = "_PlayerContext",
    typeId = 47,
    Create = function()
        local o = {}
        o.__proto = _PlayerContext
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
    end,
    ToBBuffer = function( bb, o )
    end
}
BBuffer.Register( _PlayerContext )
--[[
鱼和子弹的基类
]]
PKG_CatchFish_MoveObject = {
    typeName = "PKG_CatchFish_MoveObject",
    typeId = 33,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_MoveObject
        o.__index = o
        o.__newindex = o

        --[[
        位于容器时的下标
        ]]
        o.indexAtContainer = 0 -- Int32
        --[[
        序列号( 当发生碰撞时用于标识 )
        ]]
        o.serial = 0 -- Int32
        --[[
        创建时的帧编号
        ]]
        o.bornFrameNumber = 0 -- Int32
        --[[
        创建时的位置
        ]]
        o.bornPos = null -- _xx_Pos
        --[[
        当前位置
        ]]
        o.pos = null -- _xx_Pos
        --[[
        每帧的移动增量
        ]]
        o.moveInc = null -- _xx_Pos
        --[[
        逻辑角度
        ]]
        o.angle = 0 -- Single
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        local ReadObject = bb.ReadObject
        o.indexAtContainer = ReadInt32( bb )
        o.serial = ReadInt32( bb )
        o.bornFrameNumber = ReadInt32( bb )
        o.bornPos = ReadObject( bb )
        o.pos = ReadObject( bb )
        o.moveInc = ReadObject( bb )
        o.angle = bb:ReadSingle()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        local WriteObject = bb.WriteObject
        WriteInt32( bb, o.indexAtContainer )
        WriteInt32( bb, o.serial )
        WriteInt32( bb, o.bornFrameNumber )
        WriteObject( bb, o.bornPos )
        WriteObject( bb, o.pos )
        WriteObject( bb, o.moveInc )
        bb:WriteSingle( o.angle )
    end
}
BBuffer.Register( PKG_CatchFish_MoveObject )
--[[
子弹
]]
PKG_CatchFish_Bullet = {
    typeName = "PKG_CatchFish_Bullet",
    typeId = 34,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Bullet
        o.__index = o
        o.__newindex = o

        --[[
        哪个玩家射的
        ]]
        o.shooter = null -- PKG_CatchFish_Player
        --[[
        金币价值( 也可理解为倍率 )
        ]]
        o.coin = 0 -- Int64
        --[[
        一次性计算出来的每帧的坐标增量( 以稳定 60 fps为大前提 )
        ]]
        o.posFrameInc = null -- _xx_Pos
        setmetatable( o, PKG_CatchFish_MoveObject.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        local ReadObject = bb.ReadObject
        o.shooter = ReadObject( bb )
        o.coin = bb:ReadInt64()
        o.posFrameInc = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.shooter )
        bb:WriteInt64( o.coin )
        WriteObject( bb, o.posFrameInc )
    end
}
BBuffer.Register( PKG_CatchFish_Bullet )
--[[
鱼
]]
PKG_CatchFish_Fish = {
    typeName = "PKG_CatchFish_Fish",
    typeId = 35,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Fish
        o.__index = o
        o.__newindex = o

        --[[
        鱼类编号
        ]]
        o.typeId = 0 -- Int32
        --[[
        逻辑角度之整数版, 创建鱼或更新角度时同步更新, 方便在 cfg 中对 offsets 查表
        ]]
        o.intAngle = 0 -- Int32
        --[[
        鱼的配置信息( 不参与网络传输, 需要根据 typeId 去 cfgs 定位手工还原 )
        ]]
        o.cfg = null -- PKG_CatchFish_FishConfig
        --[[
        当前坐标( 这个每帧都通过 += posFrameInc 的方式改变 )
        ]]
        o.pos = null -- _xx_Pos
        --[[
        出生点配置
        ]]
        o.posFrom = null -- _xx_Pos
        --[[
        消失点配置
        ]]
        o.posTo = null -- _xx_Pos
        --[[
        一次性计算出来的每帧的坐标增量( 以稳定 60 fps 为大前提 )
        ]]
        o.posFrameInc = null -- _xx_Pos
        --[[
        当前移动步数( 步数一到就认为鱼已移至目的地可以清掉了 )
        ]]
        o.moveStep = 0 -- Int32
        --[[
        总移动步数
        ]]
        o.moveStepCount = 0 -- Int32
        setmetatable( o, PKG_CatchFish_MoveObject.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        local ReadInt32 = bb.ReadInt32
        local ReadObject = bb.ReadObject
        o.typeId = ReadInt32( bb )
        o.intAngle = ReadInt32( bb )
        o.cfg = ReadObject( bb )
        o.pos = ReadObject( bb )
        o.posFrom = ReadObject( bb )
        o.posTo = ReadObject( bb )
        o.posFrameInc = ReadObject( bb )
        o.moveStep = ReadInt32( bb )
        o.moveStepCount = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        local WriteInt32 = bb.WriteInt32
        local WriteObject = bb.WriteObject
        WriteInt32( bb, o.typeId )
        WriteInt32( bb, o.intAngle )
        WriteObject( bb, o.cfg )
        WriteObject( bb, o.pos )
        WriteObject( bb, o.posFrom )
        WriteObject( bb, o.posTo )
        WriteObject( bb, o.posFrameInc )
        WriteInt32( bb, o.moveStep )
        WriteInt32( bb, o.moveStepCount )
    end
}
BBuffer.Register( PKG_CatchFish_Fish )
_xx_Random = {
    typeName = "_xx_Random",
    typeId = 36,
    Create = function()
        local o = {}
        o.__proto = _xx_Random
        o.__index = o
        o.__newindex = o

        return o
    end,
    FromBBuffer = function( bb, o )
    end,
    ToBBuffer = function( bb, o )
    end
}
BBuffer.Register( _xx_Random )
List_PKG_CatchFish_Player_ = {
    typeName = "List_PKG_CatchFish_Player_",
    typeId = 37,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_Player_
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
BBuffer.Register( List_PKG_CatchFish_Player_ )
List_PKG_CatchFish_Fish_ = {
    typeName = "List_PKG_CatchFish_Fish_",
    typeId = 38,
    Create = function()
        local o = {}
        o.__proto = List_PKG_CatchFish_Fish_
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
BBuffer.Register( List_PKG_CatchFish_Fish_ )
--[[
玩家离开( 比进入的处理优先级高 )
]]
PKG_CatchFish_Events_LeavePlayer = {
    typeName = "PKG_CatchFish_Events_LeavePlayer",
    typeId = 39,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Events_LeavePlayer
        o.__index = o
        o.__newindex = o

        --[[
        座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        ]]
        o.sitIndex = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.sitIndex = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.sitIndex )
    end
}
BBuffer.Register( PKG_CatchFish_Events_LeavePlayer )
--[[
玩家进入
]]
PKG_CatchFish_Events_JoinPlayer = {
    typeName = "PKG_CatchFish_Events_JoinPlayer",
    typeId = 40,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Events_JoinPlayer
        o.__index = o
        o.__newindex = o

        --[[
        玩家名字
        ]]
        o.name = null -- String
        --[[
        座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        ]]
        o.sitIndex = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.name = bb:ReadObject()
        o.sitIndex = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteObject( o.name )
        bb:WriteInt32( o.sitIndex )
    end
}
BBuffer.Register( PKG_CatchFish_Events_JoinPlayer )
--[[
玩家开火( 单次 )
]]
PKG_CatchFish_Events_Fire = {
    typeName = "PKG_CatchFish_Events_Fire",
    typeId = 41,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Events_Fire
        o.__index = o
        o.__newindex = o

        --[[
        座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        ]]
        o.sitIndex = 0 -- Int32
        --[[
        当时的帧编号
        ]]
        o.frameNumber = 0 -- Int32
        --[[
        子弹流水号
        ]]
        o.bulletSerial = 0 -- Int32
        --[[
        金币价值( 也可理解为倍率 )
        ]]
        o.coin = 0 -- Int64
        --[[
        射击角度( 相对于炮台自己的正方向角度 )
        ]]
        o.angle = 0 -- Single
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.sitIndex = ReadInt32( bb )
        o.frameNumber = ReadInt32( bb )
        o.bulletSerial = ReadInt32( bb )
        o.coin = bb:ReadInt64()
        o.angle = bb:ReadSingle()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.sitIndex )
        WriteInt32( bb, o.frameNumber )
        WriteInt32( bb, o.bulletSerial )
        bb:WriteInt64( o.coin )
        bb:WriteSingle( o.angle )
    end
}
BBuffer.Register( PKG_CatchFish_Events_Fire )
--[[
玩家开始开火( 连发, 仅适合帧同步服务器算法 )
]]
PKG_CatchFish_Events_FireBegin = {
    typeName = "PKG_CatchFish_Events_FireBegin",
    typeId = 42,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Events_FireBegin
        o.__index = o
        o.__newindex = o

        --[[
        座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        ]]
        o.sitIndex = 0 -- Int32
        --[[
        射击角度( 相对于炮台自己的正方向角度 )
        ]]
        o.angle = 0 -- Single
        return o
    end,
    FromBBuffer = function( bb, o )
        o.sitIndex = bb:ReadInt32()
        o.angle = bb:ReadSingle()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.sitIndex )
        bb:WriteSingle( o.angle )
    end
}
BBuffer.Register( PKG_CatchFish_Events_FireBegin )
--[[
玩家开始开火( 连发, 仅适合帧同步服务器算法 )
]]
PKG_CatchFish_Events_FireChangeAngle = {
    typeName = "PKG_CatchFish_Events_FireChangeAngle",
    typeId = 43,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Events_FireChangeAngle
        o.__index = o
        o.__newindex = o

        --[[
        座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        ]]
        o.sitIndex = 0 -- Int32
        --[[
        射击角度( 相对于炮台自己的正方向角度 )
        ]]
        o.angle = 0 -- Single
        return o
    end,
    FromBBuffer = function( bb, o )
        o.sitIndex = bb:ReadInt32()
        o.angle = bb:ReadSingle()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.sitIndex )
        bb:WriteSingle( o.angle )
    end
}
BBuffer.Register( PKG_CatchFish_Events_FireChangeAngle )
--[[
玩家停止开火( 连发, 仅适合帧同步服务器算法 )
]]
PKG_CatchFish_Events_FireEnd = {
    typeName = "PKG_CatchFish_Events_FireEnd",
    typeId = 44,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Events_FireEnd
        o.__index = o
        o.__newindex = o

        --[[
        座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        ]]
        o.sitIndex = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.sitIndex = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.sitIndex )
    end
}
BBuffer.Register( PKG_CatchFish_Events_FireEnd )
--[[
子弹命中( 与鱼死分离. 鱼死相关可能要等服务器跨线程回调送回结果才能下发 )
]]
PKG_CatchFish_Events_BulletHit = {
    typeName = "PKG_CatchFish_Events_BulletHit",
    typeId = 45,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Events_BulletHit
        o.__index = o
        o.__newindex = o

        --[[
        座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        ]]
        o.sitIndex = 0 -- Int32
        --[[
        子弹流水号
        ]]
        o.bulletSerial = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.sitIndex = ReadInt32( bb )
        o.bulletSerial = ReadInt32( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.sitIndex )
        WriteInt32( bb, o.bulletSerial )
    end
}
BBuffer.Register( PKG_CatchFish_Events_BulletHit )
--[[
鱼被打死
]]
PKG_CatchFish_Events_FishDead = {
    typeName = "PKG_CatchFish_Events_FishDead",
    typeId = 46,
    Create = function()
        local o = {}
        o.__proto = PKG_CatchFish_Events_FishDead
        o.__index = o
        o.__newindex = o

        --[[
        座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        ]]
        o.sitIndex = 0 -- Int32
        --[[
        鱼流水号
        ]]
        o.fishSerial = 0 -- Int32
        --[[
        金币所得
        ]]
        o.coin = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadInt32 = bb.ReadInt32
        o.sitIndex = ReadInt32( bb )
        o.fishSerial = ReadInt32( bb )
        o.coin = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        local WriteInt32 = bb.WriteInt32
        WriteInt32( bb, o.sitIndex )
        WriteInt32( bb, o.fishSerial )
        bb:WriteInt64( o.coin )
    end
}
BBuffer.Register( PKG_CatchFish_Events_FishDead )