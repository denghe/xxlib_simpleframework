local m = scene:CreateMonster2()
local bt = os.clock()
for i = 1,100000000 do
	m:Ensure()
end
print( os.clock() - bt )




yield = coroutine.yield

function Sleep( ticks, cond )
	for i = 1, ticks do
		if cond() then return end
		yield()
	end
end

-- todo: more utils
