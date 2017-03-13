local self = ...
local yield = coroutine.yield

print( "scene.lua" )


local m = self:CreateMonster1("monster1.lua")

while true do
	print( "ticks = "..self:ticks()..", lua mem = "..collectgarbage("count")..", versionNumber = "..versionNumber() )
	if m:Ensure() then
		print( "m is alive" )
		m:x( m:x() + 1 )
	else
		print( "m is released" )
	end
	yield()
end
