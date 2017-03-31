-- 将常用类或函数本地化
local scene = _G.scene
local yield = coroutine.yield
local print = print

-- 取出外界传入的两个参数: 1. 目标类实例   2. 持久化储表
local self, store = ...

print( "scene.lua", self, store )
yield()

for i = 1, 10000 do
	local m1 = self:CreateMonster("Monster2")
	local m2 = self:CreateMonster("Monster2")
end

--[[
m1:xy( self:CreateXY(20, 20) )
m1:bornXY( self:CreateXY(20, 20) )
m2:xy( self:CreateXY(25, 25) )
m2:bornXY( self:CreateXY(25, 25) )
]]

while true do
	print( "ticks = "..self:ticks()..", monster count = "..self:Monsters_Count() ) --..", lua mem = "..collectgarbage("count") )
	--[[
	if m1:Ensure() then
		print( m1 )
	end
	if m2:Ensure() then
		print( m2 )
	end
	]]
	yield()
end


--[[
local m2 = self:CreateMonster("Monster2")
if not m2:Ensure() then
	print( "Monster2 create failed." )
end

while true do
	print( "ticks = "..self:ticks()..", lua mem = "..collectgarbage("count") )
	if m1:Ensure() then
		print( "m1 is alive" )
		m1:x( m1:x() + 1 )
	else
		print( "m1 is released" )
	end

	if m2:Ensure() then
		print( "m2 is alive" )
		m2:x( m2:x() + 1 )
	else
		print( "m2 is released" )
	end

	yield()
end
]]
