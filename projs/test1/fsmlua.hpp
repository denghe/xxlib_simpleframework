template<typename T>
FSMLua::FSMLua(T* owner, char const* codeOrFile, bool isFile)
	: FSMBase(owner)
	, err(mempoolbase())
{
	co = xx::Lua_RegisterCoroutine(this->scene().L, this);
	if (isFile)
	{
		luaL_loadfile(co, codeOrFile);
	}
	else
	{
		luaL_dostring(co, codeOrFile);
	}
	xx::LuaFunc<MP, T*>::Push(co, owner);
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
	xx::Lua_UnregisterCoroutine(this->scene().L, this);
	co = nullptr;
}
