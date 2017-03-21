-- 取出外界传入的两个参数: 1. 目标类实例   2. 持久化储表
local self, store = ...

-- 将常用类或函数本地化
local yield = coroutine.yield
local scene = _G.scene

print( "monster2.lua" )


local x = self:x()				-- bak
Sleep( 10, function() 
	return self:x() - x > 3;	-- x 有较大变化则 sleep 终止
end )
