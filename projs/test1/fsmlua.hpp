lua_State* FSMLua::GetL() const
{
	return this->scene().L;
}
template<typename T>
FSMLua::FSMLua(T* owner, char const* fn) : FSMBase(owner)
{
	mempool<MP>().CreateTo(err);
	auto L = GetL();
	co = xx::Lua_RegisterCoroutine(L, this);
	luaL_loadfile(co, fn);
	xx::Lua<MP, T*>::Push(co, owner);
	xx::Lua_Resume(co, err, 1);
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
