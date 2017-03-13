local self = ...
local yield = coroutine.yield
local scene = _G.scene

print( "monster1.lua" )


local x = self:x()				-- bak
Sleep( 10, function() 
	return self:x() - x > 3;	-- x 有较大变化则 sleep 终止
end )
