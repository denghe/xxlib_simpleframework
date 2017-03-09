local m = scene:CreateMonster1("monster1.lua")

while true do
	print( "ticks = "..scene:ticks() )
	if m:Ensure() then
		print( "m is alive" )
		m:x( m:x() + 1 )
	else
		print( "m is released" )
	end
	yield()
end
