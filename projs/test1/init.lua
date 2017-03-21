print( "init.lua" )
local yield = coroutine.yield

-- 这个只适合 AI, 没有持久化状态记录
function Sleep( ticks, cond )
	for i = 1, ticks do
		if cond() then return end
		yield()
	end
end

-- todo: more utils
