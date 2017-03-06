lua_State* FSMLua::GetL() const
{
	return ((Scene*)this->owner->sceneBase)->L;
}
FSMLua::FSMLua(char const* luacode)
{
	mempool<MP>().CreateTo(err);
	auto L = GetL();
	// 将 owner 丢到 _G[ tar->versionNumber() ]
	xx::Lua_SetGlobal<MP>(L, (lua_Integer)owner->versionNumber(), owner);
	// 创建协程
	co = xx::Lua_RegisterCoroutine(L, this);
	// 拼接定位到 owner 的指令
	err->Append("local self = _G[", owner->versionNumber(), "]\n");
	err->Append(luacode);
	// 加载协程 lua 代码
	luaL_loadstring(co, err->C_str());
}

int FSMLua::Update()
{
	auto rtv = xx::Lua_Resume(co, err);
	if (rtv == -1) std::cout << err->C_str() << std::endl;
	return rtv;
}

FSMLua::~FSMLua()
{
	xx::Lua_UnregisterCoroutine(GetL(), this);
	co = nullptr;
	if (err)
	{
		err->Release();
		err = nullptr;
	}
}
