require "PKG_class"

-- local success, ibb = pcall(function() return bb:ReadRoot() end)

function Sleep(n)
   local t0 = os.clock()
   while os.clock() - t0 <= n do end
end

local c = UvTcpClient.Create()
print(c)
for i = 1, 999 do
	Sleep(1);
	print( "." )
end
