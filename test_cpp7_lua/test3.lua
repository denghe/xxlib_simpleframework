

local t1 = {}
t1.foo1 = function() end
local t2 = {}
t2.__index = t2
t2.foo2 = function() end
setmetatable( t1, t2 )
local d1 = os.clock()
for i=0, 10000000 do t1.foo1() end
print( os.clock() - d1)
local d1 = os.clock()
for i=0, 10000000 do rawget( t1, "foo1" )() end
print( os.clock() - d1)
local d1 = os.clock()
for i=0, 10000000 do t1[ "foo1" ]() end
print( os.clock() - d1)





local t1 = {}
local d1 = os.clock()
local rs = rawset
for i=0, 10000000 do rs( t1, i, i ) end
print( os.clock() - d1)

local t2 = {}
local d1 = os.clock()
for i=0, 10000000 do t2[i] = i end
print( os.clock() - d1)


local t3 = {}
for i=0, 10000000 do t3[i] = i end

local t4 = {}
for i=0, 10000000 do t4[i] = i end


--[[
local d1 = os.clock()
local i = #t1
while i >= 0 do t1[i] = nil; i = i - 1 end
print( os.clock() - d1)


local d1 = os.clock()
for i=0, 10000000 do t2[i] = nil end
print( os.clock() - d1)


local d1 = os.clock()
for i=10000000, 0, -1 do t3[i] = nil end
print( os.clock() - d1)
--]]

local rs = rawset
local d1 = os.clock()
for i=#t1, 0, -1 do rs(t1, i, nil) end
print( os.clock() - d1)

local d1 = os.clock()
local tr = table.remove
for i=#t2, 0, -1 do tr(t2, i) end
print( os.clock() - d1)

local d1 = os.clock()
for i=#t3, 0, -1 do tr(t3) end
print( os.clock() - d1)


local d1 = os.clock()
for i=#t4, 0, -1 do t4[i] = nil end
print( os.clock() - d1, #t4)

local t1 = {}
for i=0, 10000000 do t1[i] = i end

local t2 = {}
for i=0, 10000000 do t2[i] = i end

local t3 = {}
for i=0, 10000000 do t3[i] = i end

local t4 = {}
for i=0, 10000000 do t4[i] = i end


--[[
local d1 = os.clock()
local i = #t1
while i >= 0 do t1[i] = nil; i = i - 1 end
print( os.clock() - d1)


local d1 = os.clock()
for i=0, 10000000 do t2[i] = nil end
print( os.clock() - d1)


local d1 = os.clock()
for i=10000000, 0, -1 do t3[i] = nil end
print( os.clock() - d1)
--]]

local rs = rawset
local d1 = os.clock()
for i=#t1, 0, -1 do rs(t1, i, nil) end
print( os.clock() - d1)

local d1 = os.clock()
local tr = table.remove
for i=#t2, 0, -1 do tr(t2, i) end
print( os.clock() - d1)

local d1 = os.clock()
for i=#t3, 0, -1 do tr(t3) end
print( os.clock() - d1)


local d1 = os.clock()
for i=#t4, 0, -1 do t4[i] = nil end
print( os.clock() - d1, #t4)
