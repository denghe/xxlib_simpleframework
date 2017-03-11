template<typename T>
FSMLua::FSMLua(T* owner, char const* fn) 
	: FSMBase(owner)
	, errMH(mempoolbase())
{
	co = xx::Lua_RegisterCoroutine(this->scene().L, this);
	luaL_loadfile(co, fn);
	xx::Lua<MP, T*>::Push(co, owner);
	xx::Lua_Resume(co, &err, 1);
}

int FSMLua::Update()
{
	auto rtv = xx::Lua_Resume(co, &err);
	if (rtv == -1) std::cout << err.C_str() << std::endl;
	return rtv;
}

FSMLua::~FSMLua()
{
	xx::Lua_UnregisterCoroutine(this->scene().L, this);
	co = nullptr;
}
